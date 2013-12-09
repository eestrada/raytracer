#if !defined(SYSTEM_HPP)
#define SYSTEM_HPP

/* Include C++ standard headers */
#include <exception>
#include <stdexcept>

/* Include C standard headers */
//#include <cstdio>
//#include <cstring>
//#include <cstdlib>
#include <cmath>

/* Include POSIX headers */
#include <unistd.h>
#include <time.h>

/* Include my headers */
#include "exceptions.hpp"

namespace sys
{

class sleeper
{
public:
    sleeper(long nano=0)
    {
        t.tv_sec = nano / 1000000000;
        t.tv_nsec = nano % 1000000000; 
    }

    time_t set_seconds(time_t s)
    {
        time_t retval = t.tv_sec;
        t.tv_sec = s;

        return retval;
    }

    time_t set_nanoseconds(long n)
    {
        long retval = t.tv_nsec;
        t.tv_nsec = n;

        return retval;
    }

    void sleep(void)
    {
        timespec tmp;
        int e = nanosleep(&t, &tmp);

        if(e != 0)
        {
            throw err::system_exit("POSIX error while sleeping.", errno);
        }
    }
private:
    timespec t;
};

}//end namespace sys

#endif // end include guard
