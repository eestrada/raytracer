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
        cg::Mat4 pxform = parent->get_xform();
        // Do child transform THEN parent to get expected result
        return pxform * transform;
    }
    else
    {
        return transform;
    }
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

