#if !defined(VEC4_HPP)
#define VEC4_HPP

#include <ostream>

//#include "cg_utils.hpp"
#include "Vec3.hpp"
#include "Matrix.hpp"

namespace cg
{

struct Vec4
{
    // Member variables
    double x, y, z, w;

    // Constructors
    Vec4();
    Vec4(double v);
    Vec4(double x_in, double y_in, double z_in, double w_in);
    Vec4(const Vec3 &other);
    Vec4(const Vec3 &other, double w);
    Vec4(const Vec4 &other);

    // equals operator
    Vec4 & operator=(double v);
    Vec4 & operator=(const Vec3 &other);
    Vec4 & operator=(const Vec4 &other);

    // Conversion operators
    operator Vec3();
    Vec3 to_point3();

    // Subscript operators
    double & operator[](uint8_t index);
    const double & operator[](uint8_t index) const;

    // Color helpers
    double & r();
    const double & r() const;
    double & g();
    const double & g() const;
    double & b();
    const double & b() const;
    double & a();
    const double & a() const;
};

typedef Vec4 Clr4;
} //end namespace

cg::Vec4 operator*(const cg::Mat4 &mat, const cg::Vec4 &hv);

std::ostream & operator<<(std::ostream &out, const cg::Vec4 &hv);

#endif // end include guard
