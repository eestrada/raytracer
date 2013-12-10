#include <iostream>
#include <ostream>
#include "cg/Vec4.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

Vec4::Vec4()
    : x(0.0),y(0.0),z(0.0),w(0.0) {}

Vec4::Vec4(double v)
    : x(v),y(v),z(v),w(v) {}

Vec4::Vec4(double x_in, double y_in, double z_in, double w_in)
    : x(x_in),y(y_in),z(z_in),w(w_in) {}

Vec4::Vec4(const Vec3 &other) /* Assume a point or a color, so W equals 1.0. */
    : x(other.x),y(other.y),z(other.z),w(1.0) {}

Vec4::Vec4(const Vec3 &other, double w_in)
    : x(other.x),y(other.y),z(other.z),w(w_in) {}

Vec4::Vec4(const Vec4 &other)
    : x(other.x),y(other.y),z(other.z),w(other.w) {}

Vec4 & Vec4::operator=(double v)
{
    this->x = v;
    this->y = v;
    this->z = v;
    this->w = v;

    return *this;
}

Vec4 & Vec4::operator=(const Vec3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.x;
    this->w = 1.0;/* Assume a point or a color, so W equals 1.0. */

    return *this;
}

Vec4 & Vec4::operator=(const Vec4 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.x;
    this->w = other.w;

    return *this;
}

Vec4::operator Vec3()
{
    Vec3 v;
    v.x = this->x;
    v.y = this->y;
    v.z = this->z;

    /* Ignore W component altogether. */

    return v;
}

Vec3 Vec4::to_point3()
{
    Vec3 p(*this);

    if(this->w != 0.0)
    {
        p.x /= this->w;
        p.y /= this->w;
        p.z /= this->w;
    }

    return p;
}

double & Vec4::operator[](uint8_t index)
{
    switch(index)
    {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
        case 3: return this->w;
        default: throw err::exception("Vec4 subscript out of range");
    }
}

const double & Vec4::operator[](uint8_t index) const
{
    switch(index)
    {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
        case 3: return this->w;
        default: throw err::exception("Vec4 subscript out of range");
    }
}

double & Vec4::r() {return this->x;}
const double & Vec4::r() const {return this->x;}
double & Vec4::g() {return this->y;}
const double & Vec4::g() const {return this->y;}
double & Vec4::b() {return this->z;}
const double & Vec4::b() const {return this->z;}
double & Vec4::a() {return this->w;}
const double & Vec4::a() const {return this->w;}

} //end namespace

cg::Vec4 operator*(const cg::Mat4 &mat, const cg::Vec4 &hvec)
{
    cg::Vec4 tmp(0.0);

    for(uint8_t i = 0; i < 4; ++i)
    {
        for(uint8_t j = 0; j < 4; ++j)
        {
            tmp[i] += hvec[j] * mat.at(j,i);
        }
    }
    //std::cerr << "Result of Matrix multiply is: \n" ;
    //std::cerr << mat << "     x\n" << hvec << "\n     =\n" << tmp << std::endl;
    return tmp;
}

std::ostream & operator<<(std::ostream &out, const cg::Vec4 &hv)
{
    out << "Vec4" << "( " << hv.x << " " << hv.y << " " << hv.z << " " << hv.w << " )";
    return out;
}

