#include <iostream>
#include <cmath>
#include "cg/Object.hpp"
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
    return g->intersect(ray);
}

void geo::set_geo(cg::geo_ptr g_in)
{
    this->g = g_in;
}

rt::Ray_ptr camera::get_ray(uint16_t x, uint16_t y) const
{
    double invw = 1.0/this->xres, invy = 1.0/this->yres;
    double aspect = double(this->xres)/double(this->yres);
    double angle = std::tan(PI * 0.5 * (this->fov * 2)/180.0);

    double xray = (2 * ((x + 0.5) * invw) - 1) * angle * aspect;
    double yray = (1 - 2 * ((y + 0.5) * invy)) * angle;
    cg::Vec3 pray;
    pray.x = xray;
    pray.y = yray;
    pray.z = -1;

    cg::Mat4 la = cg::matrix::lookat(this->lookfrom - this->lookat, this->lookup);

    cg::Vec3 ray = la * cg::Vec4(pray.normalized(), 0.0);

    rt::Ray_ptr rval(new rt::Ray());
    rval->dir = ray;
    rval->pos = this->lookfrom;

    return rval;
}

} // end namespace "obj"

