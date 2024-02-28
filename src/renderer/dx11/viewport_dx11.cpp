#include "renderer/dx11/viewport_dx11.h"

joj::DX11Viewport::DX11Viewport()
    : Viewport()
{
}

joj::DX11Viewport::DX11Viewport(f32 xtop_left, f32 ytop_left, u16 width, u16 height, f32 min_depth, f32 max_depth)
    : Viewport(xtop_left, ytop_left, width, height, min_depth, max_depth)
{
}

joj::DX11Viewport::~DX11Viewport()
{
}