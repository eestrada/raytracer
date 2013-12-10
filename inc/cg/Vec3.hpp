#if !defined(VEC3_HPP)
#define VEC3_HPP

#include <ostream>

namespace cg
{

struct Vec3
{
    double x, y, z;

    Vec3(double val=0.0);
    Vec3(const Vec3 &other);

    double length() const;
    double dot(const Vec3 &other) const;
    Vec3 cross(const Vec3 &other) const;
    Vec3 normalized() const;

    // Color helpers
    double & r();
    const double & r() const;
    double & g();
    const double & g() const;
    double & b();
    const double & b() const;
};

typedef Vec3 Clr3;
} //end namespace

// Output convenience functions
std::ostream & operator<<(std::ostream &out, const cg::Vec3 &v);

#endif // end include guard

