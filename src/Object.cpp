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

cg::Clr3 light::contribution(const cg::Vec3 &position)
{
    // Just return the light color. Ambient lights are so easy. :)
    return lgt_clr;
}

cg::Clr3 dist_lght::contribution(const cg::Vec3 &position)
{
    // Lazily return our parent's implementation for now.
    return light::contribution(position);
}

rt::RayHit geo::trace(const rt::Ray &ray)
{
    throw err::not_implemented("rt::RayHit geo::trace(const rt::Ray &ray)");
    return rt::RayHit();
}

void geo::set_geo(std::shared_ptr<cg::Geometry> g_in)
{
    this->g = g_in;
}

} // end namespace obj

