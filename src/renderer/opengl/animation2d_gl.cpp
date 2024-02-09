#include "renderer/opengl/animation2d_gl.h"

joj::GLAnimation2D::GLAnimation2D()
    : m_frames{std::vector<GLTextureRect>{}}, texture{nullptr}
{
}

joj::GLAnimation2D::~GLAnimation2D()
{
}

void joj::GLAnimation2D::add_frame(GLTextureRect rect)
{
    m_frames.push_back(rect);
}
