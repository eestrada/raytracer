#if !defined(GEOMETRY_HPP)
#define GEOMETRY_HPP

#include <ostream>
#include <vector>

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Matrix.hpp"
#include "rt/Ray.hpp"

namespace cg
{

struct Geometry
{
    double phong;
    Clr3 diffuse, specular, reflection;
    bool reflect;
    
    virtual Clr4 trace(const rt::Ray &ray) = 0;
};

struct Triangle : public Geometry
{
    Vec3 pt0, pt1, pt2;
    virtual Clr4 trace(const rt::Ray &ray);
};

struct Sphere : public Geometry
{
    double radius;
    Vec3 center;
    virtual Clr4 trace(const rt::Ray &ray);
};

struct TriMesh : public Geometry
{
    std::vector<Triangle> mesh;
    virtual Clr4 trace(const rt::Ray &ray);
};

} //end namespace

std::ostream & operator<<(std::ostream &out, const cg::Triangle &t);

std::ostream & operator<<(std::ostream &out, const cg::TriMesh &tm);

std::ostream & operator<<(std::ostream &out, const cg::Sphere &s);

#endif //finish include guard

