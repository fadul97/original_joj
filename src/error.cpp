#include "error.h"

#include <sstream>
#include <iostream>

const char* joj::error_to_string(joj::ErrorCode e)
{
    switch (e)
    {
        case ErrorCode::OK: return "OK";
        case ErrorCode::FAILED: return "FAILED";
        case ErrorCode::ERR_ENGINE_INIT: return "ERR_ENGINE_INIT";
        case ErrorCode::ERR_PLATFORM_MANAGER_INIT: return "ERR_PLATFORM_MANAGER_INIT";
        case ErrorCode::ERR_PLATFORM_MANAGER_CREATION: return "ERR_PLATFORM_MANAGER_CREATION";
        case ErrorCode::ERR_WINDOW_INIT: return "ERR_WINDOW_INIT";
        case ErrorCode::ERR_WINDOW_CREATION: return "ERR_WINDOW_CREATION";
        case ErrorCode::ERR_WINDOW_ID_CREATION: return "ERR_WINDOW_ID_CREATION";
        case ErrorCode::ERR_WINDOW_DISPLAY: return "ERR_WINDOW_DISPLAY";
        case ErrorCode::ERR_RENDERER_INIT: return "ERR_RENDERER_INIT";
        case ErrorCode::ERR_RENDERER_PIPELINE_ERROR: return "ERR_RENDERER_PIPELINE_ERROR";
        case ErrorCode::ERR_CONTEXT_INIT: return "ERR_CONTEXT_INIT";
        case ErrorCode::ERR_CONTEXT_CREATION: return "ERR_CONTEXT_CREATION";
        case ErrorCode::ERR_CONTEXT_GLX_VERSION: return "ERR_CONTEXT_GLX_VERSION";
        case ErrorCode::ERR_CONTEXT_FB_UNMATCH: return "ERR_CONTEXT_FB_UNMATCH";
        case ErrorCode::ERR_CONTEXT_VISUAL_CREATION: return "ERR_CONTEXT_VISUAL_CREATION";
        case ErrorCode::ERR_CONTEXT_SCREEN_UNMATCH: return "ERR_CONTEXT_SCREEN_UNMATCH";
        case ErrorCode::ERR_CONTEXT_EXTENSION_STR: return "ERR_CONTEXT_EXTENSION_STR";
        case ErrorCode::ERR_RENDERER_SWAPCHAIN_CREATION: return "ERR_RENDERER_SWAPCHAIN_CREATION";
        case ErrorCode::ERR_RENDERER_SWAPCHAIN_GET_BUFFER: return "ERR_RENDERER_SWAPCHAIN_GET_BUFFER";
        case ErrorCode::ERR_RENDER_TARGET_VIEW_CREATION: return "ERR_RENDER_TARGET_VIEW_CREATION";
        case ErrorCode::ERR_RENDERER_TEXTURE2D_CREATION: return "ERR_RENDERER_TEXTURE2D_CREATION";
        case ErrorCode::ERR_RENDERER_DEPTHSTENCIL_VIEW_CREATION: return "ERR_RENDERER_DEPTHSTENCIL_VIEW_CREATION";
        case ErrorCode::ERR_RENDERER_BLEND_STATE_CREATION: return "ERR_RENDERER_BLEND_STATE_CREATION";
        case ErrorCode::ERR_RENDERER_RASTERIZER_CREATION: return "ERR_RENDERER_RASTERIZER_CREATION";
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