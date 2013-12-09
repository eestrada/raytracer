#if !defined(HVECTOR_HPP)
#define HVECTOR_HPP

#include <ostream>

//#include "cg_utils.hpp"
#include "vector3.hpp"
#include "Matrix.hpp"

namespace cg
{

struct hvector
{
    // Member variables
    double x, y, z, w;

    // Constructors
    hvector();
    hvector(double v);
    hvector(double x_in, double y_in, double z_in, double w_in);
    hvector(const vector3 &other);
    hvector(const vector3 &other, double w);
    hvector(const point3 &other);
    hvector(const hvector &other);

    // equals operator
    hvector & operator=(double v);
    hvector & operator=(const vector3 &other);
    hvector & operator=(const point3 &other);
    hvector & operator=(const hvector &other);

    // Conversion operators
    operator vector3();
    operator point3();

    // Subscript operators
    double & operator[](uint8_t index);
    const double & operator[](uint8_t index) const;
};

} //end namespace

cg::hvector operator*(const cg::Mat4x4 &mat, const cg::hvector &hvec);

std::ostream & operator<<(std::ostream &out, const cg::hvector &hv);

#endif // end include guard
