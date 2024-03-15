#include "app.h"

#include "engine.h"

// Static member
joj::Win32PlatformManager* joj::App::platform_manager = nullptr;

joj::App::App()
{
    platform_manager = Engine::platform_manager;
}

joj::App::~App()
{
}