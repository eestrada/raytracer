#if !defined(RT_RENDERER_HPP)
#define RT_RENDERER_HPP

#include <istream>
#include <ostream>

namespace rt // rt for Ray Trace
{
// "Global" variables. Hopefully everything can be object oriented instead.

// Function declarations
int render(std::istream &scene_description, std::ostream &output_image);
int main(int argc, char **argv);

} // End namespace rt

#endif /* End include guard */

