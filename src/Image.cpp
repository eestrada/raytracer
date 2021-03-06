#include <vector>
#include <sstream>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>

#include "cg/Image.hpp"
#include "cg/cg_utils.hpp"
#include "utils/exceptions.hpp"


namespace cg
{

Image::Image(uint16_t w, uint16_t h) : width(w), height(h), pixels()
    { this->set_dimensions(w, h); }

void Image::set_dimensions(uint16_t w, uint16_t h)
{
    this->width = w;
    this->height = h;

    this->pixels.resize(w * h);
    this->pixels.shrink_to_fit();
}

uint16_t Image::get_width() const { return width; }
uint16_t Image::get_height() const { return height; }
uint32_t Image::size() const { return width * height; }

const pixelf & Image::operator[](uint32_t index) const
    { return this->pixels.at(index); }

pixelf & Image::operator[](uint32_t index)
    { return this->pixels.at(index); }

const pixelf & Image::at(uint16_t x, uint16_t y) const
    { return this->pixels.at(x + (y * this->width)); }

pixelf & Image::at(uint16_t x, uint16_t y)
    { return this->pixels.at(x + (y * this->width)); }

// Nearest pixel interpolation. In other words, point filtering.
const pixelf & Image::at_uv(double u, double v) const
{
    uint16_t x, y;
    x = utils::clamp(u, 0.0, 1.0) * this->width;
    y = utils::clamp(v, 0.0, 1.0) * this->height;

    return this->pixels.at((x * this->width) + y);
}

pixelf & Image::at_uv(double u, double v)
{
    uint16_t x, y;
    x = utils::clamp(u, 0.0, 1.0) * this->width;
    y = utils::clamp(v, 0.0, 1.0) * this->height;

    return this->pixels.at((x * this->width) + y);
}

pixelb float_to_byte(const pixelf &pf, bool premult)
{
    pixelb retval;

    if(premult)
    {
        retval.r = utils::fit<double>((pf.r * pf.a), 0, 1, 0, 255);
        retval.g = utils::fit<double>((pf.g * pf.a), 0, 1, 0, 255);
        retval.b = utils::fit<double>((pf.b * pf.a), 0, 1, 0, 255);
        retval.a = utils::fit<double>((pf.a * pf.a), 0, 1, 0, 255);
    }
    else
    {
        retval.r = utils::fit<double>((pf.r), 0, 1, 0, 255);
        retval.g = utils::fit<double>((pf.g), 0, 1, 0, 255);
        retval.b = utils::fit<double>((pf.b), 0, 1, 0, 255);
        retval.a = utils::fit<double>((pf.a), 0, 1, 0, 255);
    }

    return retval;
}

const std::string PPM_header = "P6\n# Generated by Ethan's raytracer. Boo-ya!\n";

bool writePPM(const Image &img, std::ostream &out, bool premult)
{
    out << PPM_header;
    out << img.get_width() << " " << img.get_height() << "\n255\n";

    for(uint16_t y = 0; y < img.get_height(); ++y)
    {
        for(uint16_t x = 0; x < img.get_width(); ++x)
        {
            pixelb tmppxl = float_to_byte(img.at(x, y), premult);

            out.put(tmppxl.r);
            out.put(tmppxl.g);
            out.put(tmppxl.b);
        }
    }

    return true;
}

} //end namespace "cg"

