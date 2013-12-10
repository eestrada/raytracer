#include <ostream>
#include "cg/Vec3.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

Vec3::Vec3(double val): x(val), y(val), z(val){}

Vec3::Vec3(const Vec3 &other): x(other.x), y(other.y), z(other.z){}

double Vec3::length() const
{
    throw err::not_implemented("double Vec3::length() const");
    return double();
}

double Vec3::dot(const Vec3 &other) const
{
    throw err::not_implemented("double Vec3::dot(const Vec3 &other) const");
    return double();
}

Vec3 Vec3::cross(const Vec3 &other) const
{
    throw err::not_implemented("Vec3 Vec3::cross(const Vec3 &other) const");
    return Vec3();
}

Vec3 Vec3::normalized() const
{
    throw err::not_implemented("Vec3 Vec3::normalized() const");
    return Vec3();
}

}

std::ostream & operator<<(std::ostream &out, const cg::Vec3 &v)
{
    out << "Vec3" << "( " << v.x << " " << v.y << " " << v.z << " )";
    return out;
}

