#include <cmath>
#include "cg/Object.hpp"

namespace obj
{

std::shared_ptr<object> object::scene_camera = std::shared_ptr<object>();

bool null::drawable = true;

object::object() : transform(1.0), parent() {}

cg::Mat4 object::getFinalXform() const
{
    if (parent != NULL)
    {
        cg::Mat4 pxform = parent->getFinalXform();
        // Do child transform THEN parent to get expected result
        return pxform * transform;
    }
    else
    {
        return transform;
    }
}

void object::set_geo(std::shared_ptr<cg::Geometry> g_in){}

void geo::set_geo(std::shared_ptr<cg::Geometry> g_in)
{
    this->g = g_in;
}

void geo::draw()
{
}

void null::draw()
{
}

camera::camera() : fovy(45.0), aspect(1.0), znear(0.01), zfar(100.0), perspective(1.0) {}

void camera::draw(){}

void camera::build_perspective()
{
    double f = 1 / std::tan(fovy/2);
    perspective = cg::Mat4(0);

    perspective.at(0,0) = f/aspect;
    perspective.at(1,1) = f;
    perspective.at(2,2) = (zfar + znear)/(znear - zfar);
    perspective.at(3,2) = (2 * zfar * znear)/(znear - zfar);
    perspective.at(2,3) = -1.0;
}

void light::draw()
{
}

} // end namespace obj

