#include <memory>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include "utils/exceptions.hpp"
#include "cg/Image.hpp"
#include "rt/Renderer.hpp"
#include "cg/Object.hpp"

namespace rt // rt for Ray Trace
{
// "Global" variables. Hopefully everything can be object oriented instead.
const uint8_t MAX_RAY_DEPTH = 3;

// Function declarations
void parse_material(const std::string &str, std::shared_ptr<cg::Geometry> g)
{
    std::istringstream in(str);
    std::string tmp;

    while(in.good())
    {
        tmp.clear();
        in >> tmp;

        if(tmp == "Diffuse")
        {
            in >> g->diffuse.r() >> g->diffuse.g() >> g->diffuse.b();
        }
        else if (tmp == "Reflective")
        {
            g->reflect = true;
            in >> g->reflection.r() >> g->reflection.g() >> g->reflection.b();
        }
        else if (tmp == "SpecularHighlight")
        {
            in >> g->specular.r() >> g->specular.g() >> g->specular.b();
        }
        else if (tmp == "PhongConstant")
        {
            in >> g->phong;
        }
    }
}

obj::scene parse_scene_file(std::istream &in)
{
    obj::scene scn;
    scn.scene_camera.reset(new obj::camera);
    auto cam = scn.scene_camera;

    std::shared_ptr<obj::light> dst(new obj::dist_lght);
    scn.scene_lights.push_back(dst);

    std::shared_ptr<obj::light> amb(new obj::amb_lght);
    scn.scene_lights.push_back(amb);
    std::string tmp;

    // Camera Settings
    cam->xres = cam->yres = 512;
    in >> tmp >> cam->lookat.x >> cam->lookat.y >> cam->lookat.z; // Parse cam lookat
    in >> tmp >> cam->lookfrom.x >> cam->lookfrom.y >> cam->lookfrom.z; // Parse cam lookfrom
    in >> tmp >> cam->lookup.x >> cam->lookup.y >> cam->lookup.z; // Parse cam lookup
    in >> tmp >> cam->fov; // Parse cam Field of View

    // Light Settings
    // Distant Light
    in >> tmp >> dst->clr.r() >> dst->clr.g() >> dst->clr.b();
    // Ambient Light
    in >> tmp >> amb->clr.r() >> amb->clr.g() >> amb->clr.b();
    // BG Color
    in >> tmp >> cam->bg.r() >> cam->bg.g() >> cam->bg.b();

    while(!in.eof())
    {
        tmp.clear();
        std::getline(in, tmp);

        std::istringstream strm(tmp);
        tmp.clear();

        strm >> tmp;

        if(tmp == "Sphere")
        {
            std::shared_ptr<obj::geo> tmp_geo(new obj::geo);
            std::shared_ptr<cg::Sphere> sphere(new cg::Sphere);
            tmp_geo->g = sphere;
            scn.scene_geo.push_back(tmp_geo);

            strm >> tmp >> sphere->center.x >> sphere->center.x >> sphere->center.x;
            strm >> tmp >> sphere->radius;

            tmp.clear();
            getline(strm, tmp);
            parse_material(tmp, tmp_geo->g);
        }
        else if(tmp == "Triangle")
        {
            std::shared_ptr<obj::geo> tmp_geo(new obj::geo);
            std::shared_ptr<cg::Triangle> triangle(new cg::Triangle);
            tmp_geo->g = triangle;
            scn.scene_geo.push_back(tmp_geo);

            strm >> triangle->pt0.x >> triangle->pt0.x >> triangle->pt0.x;
            strm >> triangle->pt1.x >> triangle->pt1.x >> triangle->pt1.x;
            strm >> triangle->pt2.x >> triangle->pt2.x >> triangle->pt2.x;

            tmp.clear();
            getline(strm, tmp);
            parse_material(tmp, tmp_geo->g);
        }
    }

    //std::cerr << *cam << std::endl;
    //std::cerr << rval.scene_camera->xres << "x" << rval.scene_camera->yres << std::endl;
    return scn;
}

int render(const obj::scene &scn, std::ostream &out)
{
    using cg::pixel_ctor;
    auto cam = scn.scene_camera;
    cg::Image img(cam->xres, cam->yres);

    for(uint16_t h = 0; h < img.get_height(); ++h)
    {
        for(uint16_t w = 0; w < img.get_width(); ++w)
        {
            //Compute primary ray dir
            rt::Ray pray = cam->get_ray(w, h);

            // Shoot primary ray
            double min = 1e9;
            rt::RayHit hit;
            std::shared_ptr<obj::geo> object = NULL;

            for(auto geo_ptr : scn.scene_geo)
            {
                //std::cerr <<  geo_ptr.get() << std::endl;
                auto rh = geo_ptr->trace(pray);
                if(rh.distance < min)
                {
                    min = hit.distance;
                    object = geo_ptr;
                    hit = rh;
                }
                continue;
            }

            if(hit.hit) // if we hit something
            {
                for(auto lght_ptr : scn.scene_lights) // Compute light contribution
                {
                    if(true) // Shadowed, skip the rest
                        continue;

                    // Compute diffuse
                    // compute Specular
                }
                for(auto geo_ptr : scn.scene_geo) // Compute reflections
                {
                    continue;
                }
                img.at(w,h) = pixel_ctor<float>(0.0, 0.5, 0.0, 1.0); // Green screen!
            }
            else // We didn't hit anything, set to BG color
            {
                img.at(w,h) = pixel_ctor<float>(cam->bg.r(), cam->bg.g(), cam->bg.b(), 1.0);
            }
        }
    }

    cg::writePPM(img, out);
    //out.flush();
    return 0;
}

int main(int argc, char **argv)
{
    std::unique_ptr<std::istream> tmpin;
    std::unique_ptr<std::ostream> tmpout;

    if(argc == 3)
    {
        tmpin.reset(new std::ifstream(argv[1]));
        tmpout.reset(new std::ofstream(argv[2]));
    }
    else if(argc == 2)
    {
        tmpin.reset(new std::ifstream(argv[1]));
        tmpout.reset(&std::cout);
    }
    else if(argc == 1)
    {
        tmpin.reset(&std::cin);
        tmpout.reset(&std::cout);
    }
    else
    {
        throw err::system_exit("No input or output streams could be determined.", 4);
    }

    std::istream &in = *tmpin;
    std::ostream &out = *tmpout;

    // Don't attempt to delete built in stream objects!
    if(tmpin.get() == &std::cin) tmpin.release();
    if(tmpout.get() == &std::cout) tmpout.release();

    // Parse scene
    obj::scene scn = parse_scene_file(in);

    // Render scene
    return render(scn, out);
}

} // End namespace rt

