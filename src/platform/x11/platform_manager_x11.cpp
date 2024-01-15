#include "platform/x11/platform_manager_x11.h"

#if JPLATFORM_LINUX

#include "platform/x11/window_x11.h"
#include "platform/x11/input_x11.h"
#include "graphics/context.h"
#include "graphics/x11/context_gl.h"
#include <memory>

joj::X11PlatformManager::X11PlatformManager()
{
    window = nullptr;
    input = nullptr;
    context = nullptr;
}

joj::X11PlatformManager::~X11PlatformManager()
{
}


b8 joj::X11PlatformManager::init(i16 width, i16 height, std::string title)
{
    window = std::make_unique<X11Window>(width, height, title);
    input = std::make_unique<X11Input>();
    context = std::make_unique<X11GLContext>();

    if (!window->init())
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to initialize X11Window.\n");
        return false;
    }

    if (!context->create(window))
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to initialize GLContext.\n");
        return false;
    }

    return true;
}

b8 joj::X11PlatformManager::create_window()
{
    if (!window->create())
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to create X11Window.\n");
        return false;
    }

    context->make_current(window);

    // Redirect close
    delete_msg = XInternAtom(static_cast<Display*>(window->get_display()), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(static_cast<Display*>(window->get_display()), window->get_id(), &delete_msg, 1);

    XAutoRepeatOff(static_cast<Display*>(window->get_display()));

    window->show();

    return true;
}

b8 joj::X11PlatformManager::create_simple_window(i16 width, i16 height, std::string title)
{
    window = std::make_unique<X11Window>(width, height, title);
    input = std::make_unique<X11Input>();

    if (!window->create_simple_window())
    {
        // TODO: use own logger and return value, cleanup?
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
    XEvent ev;
    char str[25];
    KeySym keysym;

    if (XPending(static_cast<Display*>(window->get_display())) > 0)
    {
        XNextEvent(static_cast<Display*>(window->get_display()), &ev);

        switch (ev.type)
        {
            case ButtonPress:
            {
                Buttons button = MAX_BUTTONS;
                switch (ev.xbutton.button)
                {
                    case 1: button = BUTTON_LEFT;
                    case 2: button = BUTTON_MIDDLE;
                    case 3: button = BUTTON_RIGHT;
                    default: break;
                }

                if (button != MAX_BUTTONS)
                    input->click_button(button);
                
                return;
            }
            case ButtonRelease:
            {
                Buttons button = MAX_BUTTONS;
                switch (ev.xbutton.button)
                {
                    case 1: button = BUTTON_LEFT;
                    case 2: button = BUTTON_MIDDLE;
                    case 3: button = BUTTON_RIGHT;
                    default: break;
                }

                if (button != MAX_BUTTONS)
                    input->release_button(button);

                return;
            }

            case KeyPress:
            {
                u32 len = XLookupString(&ev.xkey, str, 25, &keysym, nullptr);
                if (len > 0)
                {
                    Keys key = input->translate_keycode(keysym);
                    input->press_key(key);
                    return;

                    // if (input->is_key_pressed(KEY_SPACE))
                    //     std::cout << "KEY_SPACE in switch!\n";

                    // if (input->is_key_down(KEY_ENTER))
                    //     std::cout << "KEY_ENTER in switch!\n";
                }
            }
            case KeyRelease:
            {
                u32 len = XLookupString(&ev.xkey, str, 25, &keysym, nullptr);
                if (len > 0)
                {
                    Keys key = input->translate_keycode(keysym);
                    input->release_key(key);
                    return;
                }
            }

            case MotionNotify:
                input->move_mouse(ev.xmotion.x, ev.xmotion.y);
                break;

            case ClientMessage:
                if (ev.xclient.data.l[0] == delete_msg)
                    window->close();

                break;

            default:
                break;
        }
    }
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
