#include "platform/window.h"

joj::Window::Window(i16 width, i16 height, std::string title)
    : width(width), height(height), title(title)
{
}

joj::Window::~Window()
{
}
