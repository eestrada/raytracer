#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include "cg/objparser.hpp"
#include "cg/Geometry.hpp"
#include "utils/exceptions.hpp"

namespace cg
{
namespace objparser
{
    /*
     * Incoming face is turned into a triangle fan and returned as a vector of triangles
     */
    std::vector<triangle> parse_ngon_line(std::istream &in)
    {
        std::vector<triangle> trivec;
        return trivec;
    }

    /*
     * The incoming face is assumed to be triangle; no checking is done in this function
     */
    triangle parse_tri_line(std::istream &in)
    {
        triangle t;
        std::string v0, v1, v2;
        int count;
        int pi, ti, ni;

        in >> v0 >> v1 >> v2;

        count = std::count(v0.begin(), v0.end(), '/');
        // Obj indices are 1-based, so compensate by decrementing the value.
        // Assume there are no normals for now
        if(count == 2)
        {
            std::sscanf( v0.c_str(), "%d/%d/%d", &pi, &ti, &ni );
            t.pos[0] = --pi;
            t.tex[0] = --ti;
            t.nml[2] = --ni;

            std::sscanf( v1.c_str(), "%d/%d/%d", &pi, &ti, &ni );
            t.pos[1] = --pi;
            t.tex[1] = --ti;
            t.nml[2] = --ni;

            std::sscanf( v2.c_str(), "%d/%d/%d", &pi, &ti, &ni );
            t.pos[2] = --pi;
            t.tex[2] = --ti;
            t.nml[2] = --ni;
        }
        else if (count == 1)
        {
            std::sscanf( v0.c_str(), "%d/%d", &pi, &ti );
            t.pos[0] = --pi;
            t.tex[0] = --ti;

            std::sscanf( v1.c_str(), "%d/%d", &pi, &ti );
            t.pos[1] = --pi;
            t.tex[1] = --ti;

            std::sscanf( v2.c_str(), "%d/%d", &pi, &ti );
            t.pos[2] = --pi;
            t.tex[2] = --ti;
        }
        else if (count == 1 && v0.length() != 0)
        {
            std::sscanf( v0.c_str(), "%d", &pi );
            t.pos[0] = --pi;

            std::sscanf( v1.c_str(), "%d", &pi );
            t.pos[1] = --pi;

            std::sscanf( v2.c_str(), "%d", &pi );
            t.pos[2] = --pi;
        }
        else
        {
            throw err::exception("obj file face indices are bad, and you should feel bad.");
        }

        return t;
    }

    std::shared_ptr<trimesh> parse(std::istream &in)
    {
        //std::ifstream objfile(fname.c_str());
        std::string line, tmp;
        std::istringstream strm;

        std::shared_ptr<trimesh> tm_ptr(new trimesh());

        while(!in.eof())
        {
            getline(in, line);
            if(line.empty()) continue; //if line is empty, restart loop

            strm.str(line);
            strm.seekg(0);

            strm >> tmp;

            if(tmp == "v")
            {
                point p;
                strm >> p.x >> p.y >> p.z;
                tm_ptr->pts.push_back(p);
            }
            else if(tmp == "vt")
            {
                texcoords st;
                //strm >> st.x >> st.y >> st.z;
                strm >> st.x >> st.y;
                tm_ptr->uvs.push_back(st);
            }
            else if(tmp == "vn")
            {
                // normals are ignored by triangles for now
                normal n;
                strm >> n.x >> n.y >> n.z;
                tm_ptr->nmls.push_back(n);
            }
            else if(tmp == "f")
            {
                triangle t = parse_tri_line(strm);
                tm_ptr->tris.push_back(t);
            }
        }

        return tm_ptr;
    }

    // Convenience function to use filename string instead of explicit stream
    std::shared_ptr<trimesh> parse_file(const std::string &fname)
    {
        std::cerr << "Parsing file: \"" << fname << "\"";
        std::ifstream fstrm;
        //fstrm.exceptions(std::ifstream::failbit);

        fstrm.open(fname.c_str());

        auto tm = parse(fstrm);
        std::cerr << " ...Done." << std::endl;

        return tm;
    }

} // end namespace objparser
} // end namespace cg

