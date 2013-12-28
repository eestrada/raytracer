#include <iostream>
#include <cmath>
#include "cg/Object.hpp"
#include "cg/cg_utils.hpp"
#include "utils/exceptions.hpp"

namespace obj
{
const static long double PI = std::acos(-1.0L);
scene object::scn = scene();

object::object() : transform(1.0), parent() {}

cg::Mat4 object::get_xform() const
{
    if (parent != NULL)
    {
        // Do child transform THEN parent to get expected result
        return parent->get_xform() * transform;
    }
    else
    {
        return transform;
    }
}

cg::Clr3_ptr light::shadow(const cg::Vec3 &position) const
{
    // Return clr (indicating full light). Ambient lights are so easy. :)
    return cg::Clr3_ptr(new cg::Clr3(this->clr));
}

cg::Vec3 light::light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const
{
    return -nml;
}

cg::Clr3_ptr dist_lght::shadow(const cg::Vec3 &position) const
{
    // Compute shadow ray
    rt::Ray shdwray;
    shdwray.pos = position;
    shdwray.dir = this->dir_to_light.normalized();

    // Shoot shadow ray
    double min = 1e8;
    rt::RayHit_ptr hit;
    std::shared_ptr<obj::geo> sh_obj;

    for(auto tmp_obj : this->scn.scene_geo)
    {
        rt::RayHit_ptr rh = tmp_obj->trace(shdwray);
        if(rh)
        {
            return cg::Clr3_ptr(); // If we hit something, then we are shadowing, so bail early.
            if(rh->distance < min)
            {
                hit = rh;
                min = hit->distance;
                sh_obj = tmp_obj;
            }
        }
    }

    // Lazily return our parent's implementation for now.
    return light::shadow(position);
}

cg::Vec3 dist_lght::light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const
{
    return -dir_to_light;
}

rt::RayHit_ptr geo::trace(const rt::Ray &ray) const
{
    cg::Mat4 xform = this->get_xform();
    //xform = this->scn.scene_camera->get_cam_xform() * xform;
    return g->intersect(ray, xform);
}

cg::Clr3_ptr geo::shade(const rt::Ray &surf, const cg::Vec3 &I) const
{
    // Color to hold result. Init to black.
    cg::Clr3_ptr rval(new cg::Clr3(0.0));
    cg::Clr3 &clr = *rval;

    // useful constants
    cg::Vec3 ii = I.normalized();
    cg::Vec3 P = surf.pos;
    cg::Vec3 N = surf.dir.normalized();

    for(auto lght_ptr : this->scn.scene_lights) // Compute light contribution
    {
        cg::Clr3_ptr unshadowed = lght_ptr->shadow(surf.pos);

        // If shadow pointer is null, we are fully in shadow, so skip the rest.
        if(unshadowed)
        {
            // Check for ambient light
            if(typeid(*lght_ptr) == typeid(obj::light))
            {
                clr += *unshadowed * this->g->diffuse;
            }
            else
            {
                // Compute diffuse
                cg::Vec3 L = lght_ptr->light_dir(P, N);
                double NdotL = N.dot(-L.normalized());
                double diff = cg::utils::clamp(NdotL, 0.0, 1.0);
                clr += *unshadowed * this->g->diffuse * diff;

                // Compute Specular
                cg::Vec3 R = 2 * (N.dot(L)) * N - L;
                double phong = std::max(0.0, ii.dot(R));
                phong = std::pow(phong, this->g->phong);
                clr += *unshadowed * this->g->specular * phong;
            }
        }
    }

    if(this->g->reflect && surf.depth < rt::MAX_RAY_DEPTH) // Whether to compute reflection
    {
        //Compute primary ray dir
        ii = -ii;
        cg::Vec3 R = 2 * (N.dot(ii)) * N - ii;
        rt::Ray refl_ray;
        refl_ray.dir = R.normalized();
        refl_ray.pos = P;

        // Shoot primary ray
        double min = 1e8;
        rt::RayHit_ptr hit;
        std::shared_ptr<obj::geo> hit_obj;

        for(auto tmp_obj : this->scn.scene_geo)
        {
            rt::RayHit_ptr rh = tmp_obj->trace(refl_ray);
            if(rh)
            {
                if(rh->distance < min)
                {
                    hit = rh;
                    min = hit->distance;
                    hit_obj = tmp_obj;
                }
            }
        }

        if(hit)
        {
            rt::Ray rtmp = hit->data;
            rtmp.pos += rtmp.dir * 1e-12;
            rtmp.depth = surf.depth + 1;
            cg::Clr3_ptr refl_clr = hit_obj->shade(rtmp, R.normalized());
            clr += *refl_clr * 0.7;
        }
        else
        {
            clr += this->scn.scene_camera->bg * this->g->reflection;
        }
    }

    return rval;
}

void geo::set_geo(cg::geo_ptr g_in)
{
    this->g = g_in;
}

// TODO: Fix primary ray generation
rt::Ray_ptr camera::get_ray(uint16_t x, uint16_t y) const
{
    double invw = 1.0/this->xres, invy = 1.0/this->yres;
    double aspect = double(this->xres)/double(this->yres);
    double angle = std::tan(PI * 0.5 * (this->fov * 2)/180.0);
    //double angle = std::tan(PI * 0.5 * (this->fov)/180.0);

    double xray = (2 * ((x + 0.5) * invw) - 1) * angle * aspect;
    double yray = (1 - 2 * ((y + 0.5) * invy)) * angle;
    cg::Vec3 pray;
    pray.x = xray;
    pray.y = yray;
    pray.z = -1;

    cg::Mat4 xform = cg::matrix::lookat(this->lookfrom - this->lookat, this->lookup);
    xform *= cg::matrix::translate(this->lookfrom.x, this->lookfrom.y, this->lookfrom.z);

    cg::Vec3 ray = xform * cg::Vec4(pray.normalized(), 0.0);
    //cg::Vec3 ray = pray.normalized();

    rt::Ray_ptr rval(new rt::Ray());
    rval->dir = ray;
    rval->pos = xform * cg::Vec4(cg::Vec3(), 1.0);
    //rval->pos = this->lookfrom;
    //rval->pos.x = rval->pos.y = 0; rval->pos.z = 0;

    return rval;
}

cg::Mat4 camera::get_cam_xform() const
{
    cg::Mat4 rval = cg::matrix::lookat(this->lookfrom - this->lookat, this->lookup);
    rval *= cg::matrix::translate(this->lookfrom.x, this->lookfrom.y, this->lookfrom.z);
    rval = cg::matrix::inverted(rval);
    return rval;
}
} // end namespace "obj"

