#if !defined(OBJECT_HPP)
#define OBJECT_HPP
#include <memory>

#include "Vec3.hpp"
#include "Matrix.hpp"
#include "Geometry.hpp"

namespace obj
{

class object
{
public: // Constructors
    object();
public: // Functions
    cg::Mat4 getFinalXform() const;

    virtual void set_geo(std::shared_ptr<cg::Geometry> g_in);
    virtual void draw() = 0; // This makes this object an abstract class

public: // Variables
    cg::Mat4 transform;
    std::shared_ptr<object> parent;
    static std::shared_ptr<object> scene_camera;
};

class geo : public object
{
public:
    virtual void set_geo(std::shared_ptr<cg::Geometry> g_in);
    virtual void draw();

    std::shared_ptr<cg::Geometry> g;
};

class null : public object
{
public:
    virtual void draw();

public: // variables
    static bool drawable;
};

class camera : public object
{
public:
    camera();
    virtual void draw();
    virtual void build_perspective();

public: // Variables
    double fovy, aspect, znear, zfar;
    cg::Mat4 perspective;
    cg::Vec3 lookat, lookfrom, lookup;
};

class light : public object
{
public:
    virtual void draw();
};

}
#endif //finish include guard

