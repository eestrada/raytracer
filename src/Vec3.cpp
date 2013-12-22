#include <ostream>
#include <cmath>
#include "cg/Vec3.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

Vec3::Vec3(double val): x(val), y(val), z(val){}

Vec3::Vec3(const Vec3 &other): x(other.x), y(other.y), z(other.z){}

double Vec3::length() const
{
    return std::sqrt(this->length2());
}

double Vec3::length2() const
{
    return this->dot(*this);
}

double Vec3::dot(const Vec3 &other) const
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const
{
    Vec3 rval;

    rval.x = this->y * other.z - this->z * other.y;
    rval.y = this->z * other.x - this->x * other.z;
    rval.z = this->x * other.y - this->y * other.x;

    return rval;
}

Vec3 Vec3::normalized() const
{
    double l2 = this->length2();
    Vec3 rval = *this;
    if( l2 > 0 )
    {
        double inv = 1.0 / std::sqrt(l2);

        rval.x *= inv;
        rval.y *= inv;
        rval.z *= inv;
    }
    return rval;
}

Vec3 Vec3::operator*(const double &v) const
{
    Vec3 rval = *this;

    rval.x *= v;
    rval.y *= v;
    rval.z *= v;

    return rval;
}

Vec3 Vec3::operator*(const Vec3 &other) const
{
    Vec3 rval = *this;

    rval *= other;

    return rval;
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
    Vec3 rval = *this;

    rval.x -= other.x;
    rval.y -= other.y;
    rval.z -= other.z;

    return rval;
}

Vec3 Vec3::operator+(const Vec3 &other) const
{
    Vec3 rval = *this;

    rval += other;

    return rval;
}
Vec3 & Vec3::operator+=(const Vec3 &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return *this;
}

Vec3 & Vec3::operator*=(const Vec3 &other)
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;

    return *this;
}

Vec3 Vec3::operator-() const
{
    Vec3 rval = *this;

    rval.x *= -1;
    rval.y *= -1;
    rval.z *= -1;

    return rval;
}

double & Vec3::r() { return this->x; }
const double & Vec3::r() const { return this->x; }
double & Vec3::g() { return this->y; }
const double & Vec3::g() const { return this->y; }
double & Vec3::b() { return this->z; }
const double & Vec3::b() const { return this->z; }

} // end namespace cg

std::ostream & operator<<(std::ostream &out, const cg::Vec3 &v)
{
    out << "Vec3" << "( " << v.x << " " << v.y << " " << v.z << " )";
    return out;
}

