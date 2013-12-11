#if !defined(IMAGE_HPP)
#define IMAGE_HPP

#include <vector>
#include <string>
#include <ostream>
#include <memory>
#include <cstdint>

#include "cg_utils.hpp"
#include "Vec4.hpp"


namespace cg
{

template <typename T>
struct pixel_type
{
    T r, g, b, a;

    typedef T chan_type;
};

template <typename T>
pixel_type<T> pixel_ctor(T r,T g,T b,T a)
{
    pixel_type<T> retval;

    retval.r = r;
    retval.g = g;
    retval.b = b;
    retval.a = a;

    return retval;
}

typedef pixel_type<uint8_t> pixelb;
typedef pixel_type<uint16_t> pixelh;
typedef pixel_type<uint32_t> pixeli;
typedef pixel_type<float> pixelf;

/*
 * Image class
 *
 * Scanline storage starting at top-left corner.
 */
struct Image
{
public: //functions
    Image(uint16_t w=0, uint16_t h=0);

    void set_dimensions(uint16_t w, uint16_t h);
    uint16_t get_width() const;
    uint16_t get_height() const;
    uint32_t size() const;

    // index into the one dimensional array
    const pixelf & operator[](uint32_t index) const;
    pixelf & operator[](uint32_t index);

    // index into the image using the x and y coordinates
    const pixelf & at(uint16_t x, uint16_t y) const;
    pixelf & at(uint16_t x, uint16_t y);

    // Nearest pixel interpolation. In other words, point filtering.
    const pixelf & at_uv(double u, double v) const;
    pixelf & at_uv(double u, double v);

protected: //variables
    uint16_t width, height;
    std::vector<pixelf> pixels;
};


struct bitmap
{
    std::vector<uint8_t> data;
    size_t width, height;

    const uint8_t *raw() const;
};

pixelb float_to_byte(const pixelf &pf, bool premult=true);

bool writePPM(const Image &img, std::ostream &out, bool premult=true);

} //end namespace cg

#endif // end include guard

