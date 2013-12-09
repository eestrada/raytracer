#if !defined(MATRIX_HPP)
#define MATRIX_HPP

#include <stdexcept>
#include <iostream>
#include <ostream>
#include <utility>

#if __cplusplus == 199711L
    #include <stdint.h>
#else
    #include <cstdint>
#endif

#include "utils/exceptions.hpp"

namespace cg
{

template <uint8_t D>
class square_matrix
{
public:
    /*
     * Constructors
     */
    square_matrix(double identity=1.0);

    /*
     * Copy Constructor
     */
    square_matrix(const square_matrix &other);

    /*
     * Move Constructor
     */
    square_matrix(square_matrix &&other);

    /*
     * Destructor
     */
    ~square_matrix();
    
    /*
     * Assignment operators
     */
    square_matrix &operator=(double identity);

    /*
     * Copy Assignment
     */
    square_matrix &operator=(const square_matrix &other);

    /*
     * Move Assignment
     */
    square_matrix &operator=(square_matrix &&other);

    /*
     * Return the raw pointer to the underlying data array
     */
    const double *raw() const;

    /*
     * Safely index into the underlying data array
     */
    double &operator[](uint8_t index);

    /*
     * Safely index into the underlying data array
     */
    const double &operator[](uint8_t index) const;

    /*
     * Index using column and row indices
     */
    double & at(uint8_t col, uint8_t row);

    /*
     * Index using column and row indices
     */
    const double & at(uint8_t col, uint8_t row) const;

    /*
     * Return the dimensions of the matrix.
     * Since it is square, width and height are identical
     */
    uint8_t dimension() const;

    /*
     * Return the dimensions of the matrix.
     * Since it is square, width and height are identical
     */
    uint16_t size() const;

    /*
     * Return a matrix that is the transpose of this one
     */
    square_matrix transposed() const;

    /*
     * Multiplicaion operators
     */
    square_matrix operator*(const square_matrix &other) const;

    square_matrix &operator*=(const square_matrix &other);

private:
    void clear(void);
    double *val_ptr;
};

typedef square_matrix<4> Mat4x4;

namespace matrix
{
/*
 * Return a matrix that is the inverse of the input matrix
 */
Mat4x4 inverted(const Mat4x4 &m);

/*
 * Rotate about an arbitrary axis
 */
Mat4x4 rotate(double x, double y, double z, double degrees);

/*
 * Rotate about the X axis
 */
Mat4x4 rotate_x(double degrees);

/*
 * Rotate about the Y axis
 */
Mat4x4 rotate_y(double degrees);

/*
 * Rotate about the Z axis
 */
Mat4x4 rotate_z(double degrees);

/*
 * Translate Mat4x4 matrix
 */
Mat4x4 translate(double x, double y, double z);

/*
 * Scale Mat4x4 matrix
 */
Mat4x4 scale(double x, double y, double z);

/*
 * Scale Mat4x4 matrix evenly in all axes
 */
Mat4x4 uniform_scale(double s);
}

} // end namespace cg

template <uint8_t D>
std::ostream & operator<<(std::ostream &out, const cg::square_matrix<D> &m);

/* include template implemenation here */
#include "Matrix.inl"

#endif //finish include guard

