#if !defined(VECTOR3_HPP)
#define VECTOR3_HPP

#include <ostream>

namespace cg
{

struct vector
{
    double x, y, z;

    vector();
    vector(double v);
    vector(const vector &other);

    double dot(const vector &other) const;
    vector cross(const vector &other) const;
    vector normalized() const;
};


struct point : public vector
{
};

struct normal : public vector
{
};

struct texcoords : public vector
{
    const double & u() const;
    double & u();

    const double & v() const;
    double & v();

    const double & w() const;
    double & w();

};

struct colorRGB : public vector
{
    const double & r() const;
    double & r();

    const double & g() const;
    double & g();

    const double & b() const;
    double & b();
};

point vec_to_pt(const vector &other);
normal vec_to_nml(const vector &other);

typedef vector vector3;
typedef point point3;
typedef texcoords uvw;
} //end namespace

// Output convenience functions
std::ostream & operator<<(std::ostream &out, const cg::point &p);

std::ostream & operator<<(std::ostream &out, const cg::normal &n);

std::ostream & operator<<(std::ostream &out, const cg::texcoords &tex);

#endif // end include guard
