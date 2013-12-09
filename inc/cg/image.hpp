#if !defined(IMAGE_HPP)
#define IMAGE_HPP

#include <vector>
#include <memory>

#include "cg_utils.hpp"
#include "hvector.hpp"


namespace cg
{

namespace image
{

template <typename T>
struct pixel
{
    T r, g, b, a;
};

typedef pixel<uint8_t> pixelb;
typedef pixel<uint16_t> pixelh;
typedef pixel<uint32_t> pixeli;
typedef pixel<float> pixelf;

struct bitmap
{
    std::vector<uint8_t> data;
    size_t width, height;

    const uint8_t *raw() const;
};

struct colorRGBA : public hvector
{
    const double & r() const;
    double & r();

    const double & g() const;
    double & g();

    const double & b() const;
    double & b();

    const double & a() const;
    double & a();

    std::vector<uint8_t> to_bytes();
};

////////////////////////////////////////////////////////////
// FUNCTION LoadPPM(char *)
// Load a binary ppm file into a bitmap and return it
////////////////////////////////////////////////////////////

bitmap loadPPM(const std::string &filename);

} //end namespace image
} //end namespace cg

#endif // end include guard

