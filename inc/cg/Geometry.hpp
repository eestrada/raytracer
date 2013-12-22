#if !defined(GEOMETRY_HPP)
#define GEOMETRY_HPP

#include <ostream>
#include <string>
#include <vector>
#include <memory>

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
    
    virtual rt::RayHit_ptr intersect(const rt::Ray &ray) const = 0;
    virtual std::string to_string(void) const = 0;
};

struct Triangle : public Geometry
{
    Vec3 pt0, pt1, pt2;
    virtual rt::RayHit_ptr intersect(const rt::Ray &ray) const;
    virtual std::string to_string(void) const;
};

struct Sphere : public Geometry
{
    double radius;
    Vec3 center;
    virtual rt::RayHit_ptr intersect(const rt::Ray &ray) const;
    virtual std::string to_string(void) const;
};

struct TriMesh : public Geometry
{
    std::vector<Triangle> mesh;
    virtual rt::RayHit_ptr intersect(const rt::Ray &ray) const;
    virtual std::string to_string(void) const;
};

typedef std::shared_ptr<Geometry> geo_ptr;
} //end namespace "cg"

std::ostream & operator<<(std::ostream &out, const cg::Geometry &g);

#endif //finish include guard

