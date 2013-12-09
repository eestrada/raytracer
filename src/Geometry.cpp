#include <GL/gl.h>
#include <iostream>
#include "cg/Geometry.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

void trimesh::draw(const Mat4x4 &mat)
{
    //throw err::exception("Draw function not yet implemented.");
    point3 pt;

    glBegin(GL_TRIANGLES);
    for(auto tcurrent = this->tris.cbegin(); tcurrent != this->tris.cend(); ++tcurrent)
    {
        // first texture coordinate position
        glTexCoord2f( this->uvs.at(tcurrent->tex[0]).u(),
                      this->uvs.at(tcurrent->tex[0]).v());
        // first vertex position
        pt = mat * hvector(this->pts.at(tcurrent->pos[0]));
        glVertex3f( pt.x, pt.y, pt.z );

        // second texture coordinate position
        glTexCoord2f( this->uvs.at(tcurrent->tex[1]).u(),
                      this->uvs.at(tcurrent->tex[1]).v());
        // second vertex position
        pt = mat * hvector(this->pts.at(tcurrent->pos[1]));
        glVertex3f( pt.x, pt.y, pt.z );

        // third texture coordinate position
        glTexCoord2f( this->uvs.at(tcurrent->tex[2]).u(),
                      this->uvs.at(tcurrent->tex[2]).v());
        // third vertex position
        pt = mat * hvector(this->pts.at(tcurrent->pos[2]));
        glVertex3f( pt.x, pt.y, pt.z );
    }
    glEnd();

    return;
}


} //end namespace

std::ostream & operator<<(std::ostream &out, const cg::triangle &t)
{
    // Add one to these for printing to obj format indexing
    out << "f" << " ";
    out << t.pos[0] + 1 << "/" << t.tex[0] + 1 << " ";
    out << t.pos[1] + 1 << "/" << t.tex[1] + 1 << " ";
    out << t.pos[2] + 1 << "/" << t.tex[2] + 1;
    return out;
}

std::ostream & operator<<(std::ostream &out, const cg::trimesh &tm)
{
    out << "# vertices" << '\n';
    for(auto pt_iter = tm.pts.cbegin(); pt_iter != tm.pts.cend(); ++pt_iter)
    {
        out << *pt_iter << '\n';
    }

    out << "# normals" << '\n';
    for(auto nml_iter = tm.nmls.cbegin(); nml_iter != tm.nmls.cend(); ++nml_iter)
    {
        out << *nml_iter << '\n';
    }

    out << "# texture coords" << '\n';
    for(auto uv_iter = tm.uvs.cbegin(); uv_iter != tm.uvs.cend(); ++uv_iter)
    {
        out << *uv_iter << '\n';
    }

    out << "# triangles" << '\n';
    for(auto tri_iter = tm.tris.cbegin(); tri_iter != tm.tris.cend(); ++tri_iter)
    {
        out << *tri_iter << '\n';
    }

    return out;
}

