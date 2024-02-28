#include "renderer/viewport.h"

joj::Viewport::Viewport()
    : m_xtop_left(0.0f), m_ytop_left(0.0f), m_width(0), m_height(0), m_min_depth(0.0f), m_max_depth(1.0f)
{
}

joj::Viewport::Viewport(f32 xtop_left, f32 ytop_left, u16 width, u16 height, f32 min_depth, f32 max_depth)
    : m_xtop_left(xtop_left), m_ytop_left(ytop_left), m_width(width), m_height(height), m_min_depth(min_depth), m_max_depth(max_depth)
{
}

joj::Viewport::~Viewport()
{
}