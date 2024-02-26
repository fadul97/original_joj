#include "renderer/opengl/renderer_gl.h"

#define JOJ_GL_DEFINE_EXTERN
#include "graphics/opengl/joj_gl.h"
#include <iostream>

joj::GLRenderer::GLRenderer()
{
}

joj::GLRenderer::~GLRenderer()
{
}

b8 joj::GLRenderer::init(std::unique_ptr<JojWindow>& window)
{
    glViewport(0, 0, window->get_width(), window->get_height());

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    return true;
}

void joj::GLRenderer::render()
{
    std::cout << "TODO()!\n";
}

void joj::GLRenderer::clear(f32 r, f32 g, f32 b, f32 a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void joj::GLRenderer::swap_buffers()
{
    std::cout << "TODO()!\n";
    // glXSwapBuffers(window->get_display(), window->get_id());
}

void joj::GLRenderer::shutdown()
{
    std::cout << "TODO()!\n";
}
