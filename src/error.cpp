#include "error.h"

#include <sstream>
#include <iostream>

const char* joj::error_to_string(joj::ErrorCode e)
{
    switch (e)
    {
        case ErrorCode::OK: return "OK";
        case ErrorCode::ERR_CONTEXT: return "ERR_CONTEXT";
        default: return "UNKNOWN";
    }
}

void joj::print_error(ErrorCode e, const std::string& func, const std::string& file, i32 line)
{
    // Position of the last slash in the file name
    size_t pos = file.find_last_of('\\');

    std::string filename = file;

    // If bar was found
    if (pos != std::string::npos)
    {
        // Only saves the file name and not the full path
        std::string filename = file.substr(pos + 1);
    }

    std::stringstream text;
    text << "[ERROR] Function: " << func
        << " failed in " << filename
        << ", line " << std::to_string(line)
        << ":\n\t" << error_to_string(e) << "\n";

    std::cout << text.str();
}

joj::Error::Error()
    : m_ecode(ErrorCode::OK), m_line(-1), m_func_name(std::string{}), m_filename(std::string{})
{
}

joj::Error::Error(ErrorCode e, const std::string& func, const std::string& file, i32 line)
    : m_ecode(e), m_line(line), m_func_name(func), m_filename(file)
{
    // Position of the last slash in the file name
    size_t pos = file.find_last_of('\\');

    // If bar was found
    if (pos != std::string::npos)
    {
        // Only saves the file name and not the full path
        m_filename = file.substr(pos + 1);
    }
}

joj::Error::~Error()
{
}

std::string joj::Error::to_string() const
{
    std::stringstream text;
    text << "[ERROR] Function: " << m_func_name
        << " failed in " << m_filename
        << ", line " << std::to_string(m_line)
        << ":\n\t" << error_to_string(m_ecode) << "\n";

    return text.str();
}

void joj::Error::what() const
{
    std::stringstream text;
    text << "[ERROR] Function: " << m_func_name
        << " failed in " << m_filename
        << ", line " << std::to_string(m_line)
        << ":\n\t" << error_to_string(m_ecode) << "\n";

    std::cout << text.str();
}