#include "app.h"

#include "engine.h"

// Static member
joj::JPlatformManager* joj::App::platform_manager = nullptr;

joj::App::App()
{
    platform_manager = Engine::platform_manager;
}

joj::App::~App()
{
}
