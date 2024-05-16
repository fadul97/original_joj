#include "resources/resource_manager.h"

#include <fstream>
#include <sstream>
#include "logger.h"

joj::ErrorCode joj::ResourceManager::create_and_build_glshader(Shader& shader, const char* vertex_path, const char* fragment_path) {
    // Open Vertex Shader file
    std::ifstream vshader_file;
    vshader_file.open(vertex_path);
    if (vshader_file.fail())
    {
        JERROR(ErrorCode::ERR_FILE_OPEN, "ERROR: Failed to open vertex shader file.");
        return ErrorCode::ERR_FILE_OPEN;
    }

    // Open Fragment Shader file
    std::ifstream fshader_file;
    fshader_file.open(fragment_path);
    if (fshader_file.fail())
    {
        JERROR(ErrorCode::ERR_FILE_OPEN, "ERROR: Failed to open fragment shader file.");
        return ErrorCode::ERR_FILE_OPEN;
    }

    // Read file's buffer contents into streams
    std::stringstream vshader_stream, fshader_stream;
    vshader_stream << vshader_file.rdbuf();
    fshader_stream << fshader_file.rdbuf();

    // Close file handlers
    vshader_file.close();
    fshader_file.close();

    // Convert stream into string
    std::string vertex_code = vshader_stream.str();
    std::string fragment_code = fshader_stream.str();

    const char* vshader_code = vertex_code.c_str();
    const char* fshader_code = fragment_code.c_str();

    shader.vshader_code = vshader_code;
    shader.fshader_code = fshader_code;

    return ErrorCode::OK;
}