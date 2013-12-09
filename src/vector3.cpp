#include <ostream>
#include "cg/vector3.hpp"

namespace cg
{

vector::vector(): x(0.0), y(0.0), z(0.0){}

vector::vector(double v): x(v), y(v), z(v){}

vector::vector(const vector &other): x(other.x), y(other.y), z(other.z){}

double vector::dot(const vector &other) const
{
    return double();
}

vector vector::cross(const vector &other) const
{
    return vector();
}

vector vector::normalized() const
{
    return vector();
}

const double & texcoords::u() const {return x;}
double & texcoords::u() {return x;}

const double & texcoords::v() const {return y;}
double & texcoords::v() {return y;}

const double & texcoords::w() const {return z;}
double & texcoords::w() {return z;}

const double & colorRGB::r() const {return x;}
double & colorRGB::r() {return x;}

const double & colorRGB::g() const {return y;}
double & colorRGB::g() {return y;}

const double & colorRGB::b() const {return z;}
double & colorRGB::b() {return z;}

point vec_to_pt(const vector &other)
{
    point p;

    p.x = other.x;
    p.y = other.y;
    p.z = other.z;

    return p;
}

normal vec_to_nml(const vector &other)
{
    normal n;

    n.x = other.x;
    n.y = other.y;
    n.z = other.z;

    return n;
}

} //end namespace

// Output convenience functions
std::ostream & operator<<(std::ostream &out, const cg::point &p)
{
    out << "v" << " " << p.x << " " << p.y << " " << p.z;
    return out;
}

std::ostream & operator<<(std::ostream &out, const cg::normal &n)
{
    out << "vn" << " " << n.x << " " << n.y << " " << n.z;
    return out;
}

std::ostream & operator<<(std::ostream &out, const cg::texcoords &tex)
{
    out << "vt" << " " << tex.x << " " << tex.y << " " << tex.z;
    return out;
}

