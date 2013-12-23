#include <memory>
#include <typeinfo>
#include <iostream>
#include <istream>
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

obj::scene & parse_scene_file(std::istream &in, obj::scene &scn)
{
    //obj::scene scn;
    scn.scene_camera.reset(new obj::camera);
    auto cam = scn.scene_camera;

    std::shared_ptr<obj::dist_lght> dst(new obj::dist_lght);
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
    in >> tmp >> dst->dir_to_light.x >> dst->dir_to_light.y >> dst->dir_to_light.z;
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

            strm >> tmp >> sphere->center.x >> sphere->center.y >> sphere->center.z;
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

            cg::Vec3 v0, v1, v2;

            strm >> v0.x >> v0.y >> v0.z;
            strm >> v1.x >> v1.y >> v1.z;
            strm >> v2.x >> v2.y >> v2.z;

            triangle->set_points(v0, v1, v2);

            tmp.clear();
            getline(strm, tmp);
            parse_material(tmp, tmp_geo->g);
        }
        else if(tmp == "ObjFile")
        {
            throw err::not_implemented("obj file parsing");
        }
    }

    return scn;
}

int render(const obj::scene &scn, std::ostream &out)
{
    using cg::pixel_ctor;
    auto cam = scn.scene_camera;
    cg::Image img(scn.scene_camera->xres, scn.scene_camera->yres);

    std::clog << "Image height: " << img.get_height() << std::endl;
    std::clog << "Image width: " << img.get_width() << std::endl;
    for(uint16_t h = 0; h < img.get_height(); ++h)
    {
        for(uint16_t w = 0; w < img.get_width(); ++w)
        {
            //Compute primary ray dir
            rt::Ray_ptr prim_ray = scn.scene_camera->get_ray(w, h);

            // Shoot primary ray
            double min = 1e8;
            rt::RayHit_ptr hit;
            std::shared_ptr<obj::geo> object;

            for(auto tmp_obj : scn.scene_geo)
            {
                rt::RayHit_ptr rh = tmp_obj->trace(*prim_ray);
                if(rh)
                {
                    if(rh->distance < min)
                    {
                        hit = rh;
                        min = hit->distance;
                        object = tmp_obj;
                    }
                }
            }

            if(hit) // We hit something!
            {
                // Color to hold result. Init to black.
                cg::Clr3 clr(0.0);
                // Add small bias
                hit->data.pos = hit->data.pos + (hit->data.dir.normalized() * 0.01);

                for(auto lght_ptr : scn.scene_lights) // Compute light contribution
                {
                    // TODO: Check for shadowing

                    cg::Clr3_ptr unshadowed = lght_ptr->shadow(hit->data.pos);

                    // If shadow pointer is null, we are fully in shadow, so skip the rest.
                    if(unshadowed)
                    {
                        //clr = object->g->diffuse;
                        //continue;
                        if(typeid(*lght_ptr) == typeid(obj::light))
                        {
                            clr += object->g->diffuse * (*unshadowed);
                            //std::clog << "found the ambient light!" << std::endl;

                            continue;
                        }
                        else
                        {
                            // Compute diffuse
                            cg::Vec3 L = lght_ptr->light_dir(hit->data.pos, hit->data.dir);
                            double NdotL = hit->data.dir.normalized().dot(-L.normalized());
                            double diff = cg::utils::clamp(NdotL, 0.0, 1.0);
                            clr += object->g->diffuse * *unshadowed * diff;
                            // TODO: Compute Specular
                        }
                    }

                }

                for(auto tmp_obj : scn.scene_geo) // Compute reflections
                {
                    if(!object->g->reflect) continue; // No reflections here. Move along...
                    
                    // TODO: Compute object reflections
                }

                img.at(w,h) = pixel_ctor<float>(clr.r(), clr.g(), clr.b(), 1.0);
            }
            else // We didn't hit anything, set to BG color
            {
                img.at(w,h) = pixel_ctor<float>(
                    scn.scene_camera->bg.r(), 
                    scn.scene_camera->bg.g(),
                    scn.scene_camera->bg.b(), 
                    1.0);
            }
        }
    }

    cg::writePPM(img, out);
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
    obj::scene &scn = parse_scene_file(in, obj::object::scn);

    // Render scene
    return render(scn, out);
}

} // End namespace "rt"

