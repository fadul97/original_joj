#include "renderer/opengl/renderer_gl.h"

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"
#include <iostream>
#include <logger.h>

joj::GLRenderer::GLRenderer()
    : bg_color{1.0f, 1.0f, 1.0f, 1.0f}
{
}

joj::GLRenderer::~GLRenderer() = default;

joj::ErrorCode joj::GLRenderer::init(WindowConfig& window)
{
    return ErrorCode::OK;
}

void joj::GLRenderer::render()
{
    JTODO();
}

void joj::GLRenderer::clear(const f32 r, const f32 g, const f32 b, const f32 a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void joj::GLRenderer::clear()
{
    glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void joj::GLRenderer::swap_buffers()
{
    JTODO();
}

void joj::GLRenderer::shutdown()
{
    JTODO();
}

joj::ErrorCode joj::GLRenderer::setup_default_pipeline(WindowConfig& window)
{
    return ErrorCode::OK;
}