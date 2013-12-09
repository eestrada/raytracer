#if !defined(EXCEPTIONS_HPP)
#define EXCEPTIONS_HPP

/* Include C++ standard headers */
#include <exception>
#include <stdexcept>
#include <string>

namespace err
{

class exception 
{
public:
    exception(const std::string &msg = "") : what_str(msg) {}
    virtual ~exception(){}

    virtual const char* what() const throw()
    {
        return what_str.c_str();
    }
protected:
    std::string what_str;
};


class system_exit : public exception
{
public:
    system_exit(const std::string &msg = "", int return_code = 0)
        : exception(msg), exit_code(return_code) {}
    virtual ~system_exit(){}

    virtual int code() const throw()
    {
        return this->exit_code;
    }
protected:
    int exit_code;
};

class not_implemented : public exception
{
public:
    not_implemented(const std::string &msg = "") : exception(msg)
    {
        this->what_str = "Not implemented: " + this->what_str;
    }
    virtual ~not_implemented(){}
protected:
};

}// end err namespace 

#endif // end include guard
