#if !defined(TRANSFORM_HPP)
#define TRANSFORM_HPP

#include "Matrix.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

class Transform
{
public:
    Transform() : tx(), ty(), tz(), rx(), ry(), rz(), sx(1), sy(1), sz(1){}
    Transform(const Transform &other) = default;

    operator Mat4x4()
    {
        Mat4x4 tmp(1);

        tmp = matrix::translate(tx, ty, tz);
        tmp *= matrix::rotate_z(rz);
        tmp *= matrix::rotate_y(ry);
        tmp *= matrix::rotate_x(rx);
        tmp *= matrix::scale(sx, sy, sz);

        return tmp;
    }

public: // Variables
    double tx, ty, tz, rx, ry, rz, sx, sy, sz;
private:
};

}
#endif // End include guard
