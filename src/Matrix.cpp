#include "cg/Matrix.hpp"
#include "cg/vector3.hpp"
#include "cg/cg_utils.hpp"
#include "utils/exceptions.hpp"
#include <iostream>
#include <cmath>

namespace cg
{

namespace matrix
{

Mat4x4 rotate_x(double degrees)
{
    Mat4x4 tmp(1.0);
    double radians = cg::utils::radians(degrees);

    tmp.at(1,1) = std::cos(radians);
    tmp.at(2,2) = std::cos(radians);
    tmp.at(1,2) = std::sin(radians);
    tmp.at(2,1) = -std::sin(radians);
    //std::cerr << "The Mat4x4 rotated along the X axis is \n" << tmp << std::endl;
    return tmp;
}

Mat4x4 rotate_y(double degrees)
{
    Mat4x4 tmp(1.0);
    double radians = cg::utils::radians(degrees);

    tmp.at(0,0) = std::cos(radians);
    tmp.at(2,2) = std::cos(radians);
    tmp.at(2,0) = std::sin(radians);
    tmp.at(0,2) = -std::sin(radians);
    //std::cerr << "The Mat4x4 rotated along the Y axis is \n" << tmp << std::endl;
    return tmp;
}

Mat4x4 rotate_z(double degrees)
{
    Mat4x4 tmp(1.0);
    double radians = cg::utils::radians(degrees);

    tmp.at(0,0) = std::cos(radians);
    tmp.at(1,1) = std::cos(radians);
    tmp.at(1,0) = -std::sin(radians);
    tmp.at(0,1) = std::sin(radians);
    //std::cerr << "The Mat4x4 rotated along the Y axis is \n" << tmp << std::endl;
    return tmp;
}

Mat4x4 translate(double x, double y, double z)
{
    Mat4x4 tmp(1.0);

    tmp.at(3, 0) = x;
    tmp.at(3, 1) = y;
    tmp.at(3, 2) = z;

    return tmp;
}

Mat4x4 scale(double x, double y, double z)
{
    Mat4x4 tmp(1.0);

    tmp.at(0, 0) = x;
    tmp.at(1, 1) = y;
    tmp.at(2, 2) = z;

    return tmp;
}

Mat4x4 uniform_scale(double s)
{
    return scale(s,s,s);
}

Mat4x4 rotate(double x, double y, double z, double degrees)
{
    throw err::not_implemented("Mat4x4 rotate(double x, double y, double z, double degrees)");

    vector v;
    v.x = x;
    v.y = y;
    v.z = z;

    v = v.normalized();
    Mat4x4 tmp(1.0);
    double radians = cg::utils::radians(degrees);
    double c = std::cos(radians);
    double s = std::sin(radians);

    tmp.at(0,0) = v.x * v.x * (1 - c) + c;
    tmp.at(1,0) = v.y * v.x * (1 - c) + v.z * s;
    // do more here
    return tmp;
}

// inversion code based on MESA implementation of the gluInvertMatrix function of the GLU library
Mat4x4 inverted(const Mat4x4 &m)
{
    Mat4x4 inv(0.0);

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    double det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
    {
        throw err::exception("Mat4x4 could not be inverted");
        return false;
    }

    det = 1.0 / det;

    Mat4x4 invOut(0);
    for (int i = 0; i < 16; ++i)
    {
        invOut[i] = inv[i] * det;
    }

    //std::cerr << "Input matrix: \n" << m << std::endl;
    //std::cerr << "Inverted matrix: \n" << invOut << std::endl;
    //std::cerr << "Multiplied by eachother: \n" << (m * invOut) << std::endl;

    return invOut;
}

}

}

