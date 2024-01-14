#include "platform/x11/platform_manager_x11.h"

#if JPLATFORM_LINUX

#include <memory>

joj::X11PlatformManager::X11PlatformManager()
{
    window = nullptr;
}

joj::X11PlatformManager::~X11PlatformManager()
{
}


b8 joj::X11PlatformManager::init(i16 width, i16 height, std::string title)
{
    window = std::make_unique<X11Window>(width, height, title);

    if (!window->init())
    {
        // TODO: use own logger and return value
        printf("Failed to initialize X11Window.\n");
        return false;
    }

    return true;
}

b8 joj::X11PlatformManager::create_window()
{
    if (!window->create())
    {
        // TODO: use own logger and return value
        printf("Failed to create X11Window.\n");
        return false;
    }

    window->show();

    return true;
}

b8 joj::X11PlatformManager::create_simple_window(i16 width, i16 height, std::string title)
{
    window = std::make_unique<X11Window>(width, height, title);

    if (!window->create_simple_window())
    {
        // TODO: use own logger and return value
        printf("Failed to create X11Window.\n");
        return false;
    }

    // Redirect close
    delete_msg = XInternAtom(static_cast<Display*>(window->get_display()), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(static_cast<Display*>(window->get_display()), window->get_id(), &delete_msg, 1);

    XAutoRepeatOff(static_cast<Display*>(window->get_display()));

    window->show();

    return true;
}

void joj::X11PlatformManager::process_events()
{
    printf("TODO()!\n");
}

void joj::X11PlatformManager::swap_buffers()
{
    printf("TODO()!\n");
}

void joj::X11PlatformManager::shutdown()
{
    XAutoRepeatOn(static_cast<Display*>(window->get_display()));
    window->shutdown();
}

#endif // JPLATFORM_LINUX
