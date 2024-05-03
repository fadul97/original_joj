#ifndef JOJ_ERROR_H
#define JOJ_ERROR_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "error_code.h"

namespace joj
{
    void print_error(ErrorCode e, const std::string& func, const std::string& file, i32 line);

    const char* error_to_string(ErrorCode e);

    class JAPI Error
    {
    public:
        Error();
        Error(ErrorCode e, const std::string& func, const std::string& file, i32 line);
        ~Error();

        std::string to_string() const;
        void what() const;

    private:
        ErrorCode m_ecode;
        std::string m_func_name;
        std::string m_filename;
        i32 m_line;
    };
}

#define JFAILED(x) (((joj::ErrorCode)(x)) != joj::ErrorCode::OK)

#ifndef JOUTPUTFAILED
#define JOUTPUTFAILED(x)                                                                                          \
{                                                                                                                 \
    joj::ErrorCode result = (x);                                                                                  \
    if(result != joj::ErrorCode::OK) { joj::print_error(result, __func__, __FILE__, __LINE__); return result; }   \
}
#endif

#endif // JOJ_ERROR_H
