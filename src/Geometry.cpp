#include <iostream>
#include <cmath>
#include "cg/Geometry.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

rt::RayHit Sphere::trace(const rt::Ray &ray)
{
    rt::RayHit rval;
    rval.hit = false;

    Vec3 OC = this->center - ray.pos;
    double tca = OC.dot(ray.dir);
    if (tca < 0) return rval;
    double d2 = OC.length2() - tca * tca;
    double radius2 = radius * radius;
    if(d2 > radius2)return rval;

    double thc = std::sqrt(radius2 - d2);
    rval.hit = true;
    rval.distance = tca - thc;
    rval.data.pos = ray.pos + ray.dir * rval.distance;
    rval.data.dir = rval.data.pos - this->center;
    rval.data.dir = rval.data.dir.normalized();

    return rval;
}

rt::RayHit Triangle::trace(const rt::Ray &ray)
{
    return rt::RayHit();
    throw err::not_implemented("rt::RayHit Triangle::trace(const rt::Ray &ray)");
}

} //end namespace

std::ostream & operator<<(std::ostream &out, const cg::Triangle &t)
{
    // Add one to these for printing to obj format indexing
    return out;
}

std::ostream & operator<<(std::ostream &out, const cg::TriMesh &tm)
{
    return out;
}

