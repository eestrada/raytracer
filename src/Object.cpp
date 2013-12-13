#include <cmath>
#include "cg/Object.hpp"
#include "utils/exceptions.hpp"

namespace obj
{

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

bool light::shadow(const cg::Vec3 &position) const
{
    // Just return a constant 1.0 (full light). Ambient lights are so easy. :)
    return false;
}

cg::Vec3 light::light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const
{
    // Just return a constant 1.0 (full light). Ambient lights are so easy. :)
    return -nml;
}

bool dist_lght::shadow(const cg::Vec3 &position) const
{
    // Lazily return our parent's implementation for now.
    return light::shadow(position);
}

cg::Vec3 dist_lght::light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const
{
    // Just return a constant 1.0 (full light). Ambient lights are so easy. :)
    return -dir_to_light;
}

rt::RayHit geo::trace(const rt::Ray &ray)
{
    return g->trace(ray);
}

void geo::set_geo(std::shared_ptr<cg::Geometry> g_in)
{
    this->g = g_in;
}

rt::Ray camera::get_ray(uint16_t x, uint16_t y) const
{
    double invw = 1.0/this->xres, invy = 1.0/this->yres;
    double aspect = double(this->xres)/double(this->yres);
    double angle = std::tan(std::acos(-1.0) * 0.5 * this->fov/180.0);

    double xray = (2 * ((x + 0.5) * invw) - 1) * angle * aspect;
    double yray = (1 - 2 * ((y + 0.5) * invy)) * angle;
    cg::Vec3 pray;
    pray.x = xray;
    pray.y = yray;
    pray.z = -1;

    rt::Ray rval;
    rval.dir = pray.normalized();
    rval.pos = this->lookfrom;
    //throw err::not_implemented("rt::Ray camera::get_ray(uint16_t x, uint16_t y) const");
    return rval;
}
} // end namespace obj

