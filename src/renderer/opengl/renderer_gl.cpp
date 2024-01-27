#include "renderer/opengl/renderer_gl.h"
#include "graphics/x11/joj_gl_x11.h"

#include <iostream>
#include <GL/gl.h>

joj::GLRenderer::GLRenderer()
{
}

joj::GLRenderer::~GLRenderer()
{
}


b8 joj::GLRenderer::init(std::unique_ptr<Window>& window)
{
    glViewport(0, 0, window->get_width(), window->get_height());
    
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

void joj::GLRenderer::shutdown()
{
    std::cout << "TODO()!\n";
}
