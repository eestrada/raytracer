#include <iostream>
#include "cg/Geometry.hpp"
#include "utils/exceptions.hpp"

namespace cg
{

} //end namespace

std::ostream & operator<<(std::ostream &out, const cg::Triangle &t)
{
    // Add one to these for printing to obj format indexing
    return out;
}

std::ostream & operator<<(std::ostream &out, const cg::TriMesh &tm)
{
    return out;
}

