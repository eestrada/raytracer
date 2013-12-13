#if !defined(OBJECT_HPP)
#define OBJECT_HPP
#include <memory>
#include <vector>
#include <ostream>

#include "Vec3.hpp"
#include "rt/Ray.hpp"
#include "Matrix.hpp"
#include "Geometry.hpp"

namespace obj
{

struct object
{
public: // Constructors
    object();
public: // Functions
    /*
     * Gets the transforms of this object AND its parent. Works recursively.
     */
    cg::Mat4 get_xform() const;


public: // Variables
    cg::Mat4 transform;
    std::shared_ptr<object> parent;
};

struct geo : public object
{
public:
    virtual void set_geo(std::shared_ptr<cg::Geometry> g_in);
    virtual rt::RayHit trace(const rt::Ray &ray);

    std::shared_ptr<cg::Geometry> g;
};

struct camera : public object
{
    uint16_t xres, yres;
    double fov, aspect, znear, zfar;
    cg::Vec3 lookat, lookfrom, lookup;
    cg::Clr3 bg;

    rt::Ray get_ray(uint16_t x, uint16_t y) const;
};

struct light : public object
{
    virtual bool shadow(const cg::Vec3 &position) const;
    virtual cg::Vec3 light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const;
    cg::Clr3 clr;
};

struct dist_lght : public light
{
    virtual bool shadow(const cg::Vec3 &position) const;
    virtual cg::Vec3 light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const;
//protected:
public:
    cg::Vec3 dir_to_light;
};

struct pt_lght : public light
{
    virtual bool shadow(const cg::Vec3 &position) const;
    virtual cg::Vec3 light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const;
protected:
    cg::Vec3 pos;
};

struct spot_lght : public pt_lght
{
    virtual bool shadow(const cg::Vec3 &position) const;
    virtual cg::Vec3 light_dir(const cg::Vec3 &pos, const cg::Vec3 &nml) const;

protected:
    cg::Vec3 dir;
    double cone_angle, penumbra;
};

typedef light amb_lght;
typedef object null;

struct scene
{
std::vector< std::shared_ptr<geo> > scene_geo;

std::vector< std::shared_ptr<light> > scene_lights;

std::shared_ptr<camera> scene_camera;
};

} // end obj namespace

inline std::ostream & operator<<(std::ostream &out, const obj::camera &cam)
{
    out << "Camera settings:\n";
    out << "Resolution " << cam.xres << "x" << cam.yres << "\n";
    out << "Field of View " << cam.fov << "\n";
    out << "Look At " << cam.lookat << "\n";
    out << "Look From " << cam.lookfrom << "\n";
    out << "Look Up " << cam.lookup << "\n";

    return out;
}

#endif //finish include guard

