#if !defined(UTILS_HPP)
#define UTILS_HPP

/* Include C++ standard headers */
#include <fstream>
#include <sstream>
#include <string>


std::string load_file_as_str(const std::string &fname)
{
        std::ifstream file;
        file.exceptions(std::ifstream::failbit);

        try
        {
            file.open(fname.c_str());
        }
        catch(std::ios_base::failure &e)
        {
            std::cerr << "When opening file \"" << fname;
            std::cerr << "\" the following exception occured: ";
            std::cerr << e.what() << std::endl;

            throw e;
        }
 
        std::stringstream fileData;
        fileData << file.rdbuf();
        file.close();
 
        return fileData.str();
}

#endif // end include guard

