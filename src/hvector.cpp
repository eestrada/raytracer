#include <iostream>
#include <ostream>
#include "cg/hvector.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

hvector::hvector()
    : x(0.0),y(0.0),z(0.0),w(0.0) {}

hvector::hvector(double v)
    : x(v),y(v),z(v),w(v) {}

hvector::hvector(double x_in, double y_in, double z_in, double w_in)
    : x(x_in),y(y_in),z(z_in),w(w_in) {}

hvector::hvector(const vector3 &other)
    : x(other.x),y(other.y),z(other.z),w(0.0) {}

hvector::hvector(const vector3 &other, double w_in)
    : x(other.x),y(other.y),z(other.z),w(w_in) {}

hvector::hvector(const point3 &other)
    : x(other.x),y(other.y),z(other.z),w(1.0) {}

hvector::hvector(const hvector &other)
    : x(other.x),y(other.y),z(other.z),w(other.w) {}

hvector::operator vector3()
{
    vector3 v;
    v.x = this->x;
    v.y = this->y;
    v.z = this->z;

    return v;
}

hvector::operator point3()
{
    point3 p(vec_to_pt(this->operator vector3()));

    if(this->w != 0.0)
    {
        p.x /= this->w;
        p.y /= this->w;
        p.z /= this->w;
    }

    return p;
}

double & hvector::operator[](uint8_t index)
{
    switch(index)
    {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
        case 3: return this->w;
        default: throw err::exception("hvector subscript out of range");
    }
}

const double & hvector::operator[](uint8_t index) const
{
    switch(index)
    {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
        case 3: return this->w;
        default: throw err::exception("hvector subscript out of range");
    }
}

} //end namespace

cg::hvector operator*(const cg::Mat4x4 &mat, const cg::hvector &hvec)
{
    cg::hvector tmp(0.0);

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

std::ostream & operator<<(std::ostream &out, const cg::hvector &hv)
{
    out << "hvector" << " " << hv.x << " " << hv.y << " " << hv.z << " " << hv.w;
    return out;
}

