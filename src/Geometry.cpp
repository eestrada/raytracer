#include <iostream>
#include <sstream>
#include <cmath>
#include "cg/Geometry.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

rt::RayHit_ptr Sphere::intersect(const rt::Ray &ray) const
{
    Vec3 L = this->center - ray.pos;
    double tca = L.dot(ray.dir.normalized());
    if (tca < 0) return rt::RayHit_ptr(); // We already know if the intersection is behind us.

    double d2 = L.length2() - tca * tca;
    double radius2 = this->radius * this->radius;
    if(d2 > radius2) return rt::RayHit_ptr(); // The ray angle is greater than the radius.

    // If we made it this far, we have successfully intersected.
    double thc = std::sqrt(radius2 - d2);

    rt::RayHit_ptr rval(new rt::RayHit());
    rval->distance = tca - thc;
    rval->data.pos = ray.pos + ray.dir.normalized() * rval->distance;
    rval->data.dir = (rval->data.pos - this->center).normalized();

    return rval;
}

std::string Sphere::to_string(void) const
{
    std::ostringstream out;
    out << "Sphere: Center " << this->center << " radius " << this->radius; 
    return out.str();
}

void Triangle::set_points(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
{
    Vec3 A = v1 - v0;
    Vec3 B = v2 - v0;
    Vec3 C = A.cross(B); // CCW winding of points

    this->nml = C.normalized();
    this->pt0 = v0;
    this->pt1 = v1;
    this->pt2 = v2;
}

rt::RayHit_ptr Triangle::intersect(const rt::Ray &ray) const
{
    rt::RayHit_ptr rval;

    double denom = ray.dir.dot(this->nml);
    if (denom == 0.0) return rval; // We are perfectly perpendicular to the triangle.

    Vec3 ptro = pt0 - ray.pos;
    double d = ptro.dot(this->nml) / denom;

    if(d < 0) return rval; // Is the triangle in front of the camera?

    Vec3 P =  ray.pos + (ray.dir.normalized() * d);
    Vec3 edge0 = this->pt1 - this->pt0;
    Vec3 edge1 = this->pt2 - this->pt1;
    Vec3 edge2 = this->pt0 - this->pt2;
    Vec3 C0 = P - this->pt0;
    Vec3 C1 = P - this->pt1;
    Vec3 C2 = P - this->pt2;

    if( this->nml.dot(edge0.cross(C0)) > 0 &&
        this->nml.dot(edge1.cross(C1)) > 0 &&
        this->nml.dot(edge2.cross(C2)) > 0) // Is intersection within the triangle face?
    {
        rval.reset(new rt::RayHit());
        rval->distance = d;
        rval->data.dir = this->nml;
        rval->data.pos = P;
    }

    return rval;
}

std::string Triangle::to_string(void) const
{
    return std::string("Printing a Triangle!");
}

} //end namespace "cg"

std::ostream & operator<<(std::ostream &out, const cg::Geometry &g)
{
    return out << g.to_string() << "\n";
}
