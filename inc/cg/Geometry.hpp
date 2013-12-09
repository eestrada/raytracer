#if !defined(GEOMETRY_HPP)
#define GEOMETRY_HPP

#include <ostream>
#include <vector>

#include "vector3.hpp"
#include "hvector.hpp"
#include "Matrix.hpp"

namespace cg
{

struct Geometry
{
    std::vector<point3> pts;
    virtual void draw(const Mat4x4 &mat) = 0;
};


struct triangle
{
    int pos[3];
    int nml[3];
    int tex[3];
};

struct trimesh : public Geometry
{
    std::vector<normal> nmls;
    std::vector<uvw> uvs;
    std::vector<triangle> tris;

    virtual void draw(const Mat4x4 &mat);
};

} //end namespace

std::ostream & operator<<(std::ostream &out, const cg::triangle &t);

std::ostream & operator<<(std::ostream &out, const cg::trimesh &tm);

#endif //finish include guard

