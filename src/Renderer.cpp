#include <memory>
#include <iostream>
#include <ostream>
#include <fstream>
#include "utils/exceptions.hpp"
#include "rt/Renderer.hpp"

namespace rt // rt for Ray Trace
{
// "Global" variables. Hopefully everything can be object oriented instead.

// Function declarations
int render(std::istream &in, std::ostream &out)
{
    out << in.rdbuf();
    out.flush();
    return 0;
}

int main(int argc, char **argv)
{
    std::unique_ptr<std::istream> tmpin;
    std::unique_ptr<std::ostream> tmpout;

    if(argc == 3)
    {
        tmpin.reset(new std::ifstream(argv[1]));
        tmpout.reset(new std::ofstream(argv[2]));
    }
    else if(argc == 2)
    {
        tmpin.reset(new std::ifstream(argv[1]));
        tmpout.reset(&std::cout);
    }
    else if(argc == 1)
    {
        tmpin.reset(&std::cin);
        tmpout.reset(&std::cout);
    }
    else
    {
        throw err::system_exit("No input or output streams could be determined.", 2);
    }

    std::istream &in = *tmpin;
    std::ostream &out = *tmpout;

    // Don't attempt to delete built in stream objects!
    if(tmpin.get() == &std::cin) tmpin.release();
    if(tmpout.get() == &std::cout) tmpout.release();

    return render(in, out);
}
} // End namespace rt

