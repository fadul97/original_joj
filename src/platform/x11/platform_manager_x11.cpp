#include "platform/x11/platform_manager_x11.h"

#if JPLATFORM_LINUX

#include "platform/x11/window_x11.h"
#include "platform/x11/input_x11.h"
#include "platform/context/context.h"
#include "platform/context/opengl/x11/context_gl.h"
#include <memory>
#include <X11/Xatom.h>

joj::X11PlatformManager::X11PlatformManager()
{
    m_window = nullptr;
    m_input = nullptr;
    m_context = nullptr;
}

joj::X11PlatformManager::~X11PlatformManager()
{
}

joj::ErrorCode joj::X11PlatformManager::create_window(i16 width, i16 height, const char* title, WindowMode mode)
{
    m_window = std::make_unique<X11Window>(width, height, title);
    m_input = std::make_unique<X11Input>();
    m_context = std::make_unique<X11GLContext>();

    if (!m_window->init())
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to initialize X11Window.\n");
        return ErrorCode::ERR_WINDOW_INIT;
    }

    if (!m_context->create(m_window))
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to initialize GLContext.\n");
        return ErrorCode::ERR_CONTEXT_CREATION;
    }

    if (!m_window->create())
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to create X11Window.\n");
        return ErrorCode::ERR_WINDOW_CREATION;
    }

    m_context->make_current(m_window);

    // Redirect close
    delete_msg = XInternAtom(m_window->get_display(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_window->get_display(), m_window->get_id(), &delete_msg, 1);

    XAutoRepeatOff(m_window->get_display());

    m_window->show();

    return ErrorCode::OK;
}

std::unique_ptr<joj::X11Window> joj::X11PlatformManager::create_simple_window(i16 width, i16 height, const char* title)
{
    auto new_window = std::make_unique<X11Window>(width, height, title);

    if (!new_window->create_simple_window())
    {
        // TODO: use own logger and return value, cleanup?
        printf("Failed to create X11Window.\n");
        return nullptr;
    }

    // Redirect close
    delete_msg = XInternAtom(new_window->get_display(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(new_window->get_display(), new_window->get_id(), &delete_msg, 1);

    XAutoRepeatOff(new_window->get_display());

    new_window->show();

    return new_window;
}

joj::ErrorCode joj::X11PlatformManager::create_input()
{
    return ErrorCode::FAILED;
}

joj::ErrorCode joj::X11PlatformManager::create_context(std::unique_ptr<X11Window>& window, BackendRenderer backend_renderer)
{
    switch (backend_renderer)
    {
    default:
        if (!m_context->create(window))
        {
            // TODO: use own logger and return value, cleanup?
            printf("Failed to initialize GLContext.\n");
            return ErrorCode::ERR_CONTEXT_CREATION;
        }

        return ErrorCode::OK;
    }
}

joj::ErrorCode joj::X11PlatformManager::create_window_and_context(std::unique_ptr<X11Window>& window, BackendRenderer backend_renderer)
{
    return ErrorCode::FAILED;
}

joj::ErrorCode joj::X11PlatformManager::make_gl_context_current(std::unique_ptr<X11Window>& window)
{
    return ErrorCode::FAILED;
}

joj::ErrorCode joj::X11PlatformManager::create_timer()
{
    return ErrorCode::FAILED;
}

b8 joj::X11PlatformManager::process_events()
{
    XEvent ev;
    char str[25];
    KeySym keysym;

    if (XPending(m_window->get_display()) > 0)
    {
        XNextEvent(m_window->get_display(), &ev);

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
                {
                    m_input->click_button(button);
                    return true;
                }
                
                return false;
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
                {
                    m_input->release_button(button);
                    return true;
                }

                return false;
            }

            case KeyPress:
            {
                u32 len = XLookupString(&ev.xkey, str, 25, &keysym, nullptr);
                if (len > 0)
                {
                    Keys key = m_input->translate_keycode(keysym);
                    m_input->press_key(key);
                    return true;

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
                    Keys key = m_input->translate_keycode(keysym);
                    m_input->release_key(key);
                    return true;
                }
            }

            case MotionNotify:
                m_input->move_mouse(ev.xmotion.x, ev.xmotion.y);
                break;

            case ClientMessage:
                if (ev.xclient.data.l[0] == delete_msg)
                    return false;

                break;

            default:
                break;
        }
    }

    return false;
}

void joj::X11PlatformManager::swap_buffers()
{
    glXSwapBuffers(m_window->get_display(), m_window->get_id());
}

void joj::X11PlatformManager::shutdown()
{
    XAutoRepeatOn(m_window->get_display());
    m_window->shutdown();
}

void joj::X11PlatformManager::set_window_icon(std::unique_ptr<X11Window>& window, i32 count, IconImage& image)
{
    if (count)
    {
        i32 longCount = 0;

        for (i32 i = 0;  i < count;  i++)
            longCount += 2 + image.width * image.height;

        unsigned long* icon = static_cast<unsigned long*>(calloc(longCount, sizeof(unsigned long)));
        unsigned long* target = icon;

        for (i32 i = 0;  i < count;  i++)
        {
            *target++ = image.width;
            *target++ = image.height;

            for (i32 j = 0;  j < image.width * image.height;  j++)
            {
                *target++ = (((unsigned long) image.pixels[j * 4 + 0]) << 16) |
                            (((unsigned long) image.pixels[j * 4 + 1]) <<  8) |
                            (((unsigned long) image.pixels[j * 4 + 2]) <<  0) |
                            (((unsigned long) image.pixels[j * 4 + 3]) << 24);
            }
        }

        XChangeProperty(window->get_display(), window->get_id(),
                        XInternAtom(window->get_display(), "_NET_WM_ICON", False),
                        XA_CARDINAL, 32,
                        PropModeReplace,
                        (unsigned char*) icon,
                        longCount);

        free(icon);
    }
    else
    {
        XDeleteProperty(window->get_display(), window->get_id(),
                        XInternAtom(window->get_display(), "_NET_WM_ICON", False));
    }

    XFlush(window->get_display());
}

void joj::X11PlatformManager::change_window_procedure(std::unique_ptr<X11Window>& window, void* func)
{
    printf("TODO()!\n");
}

void joj::X11PlatformManager::set_window_title(std::unique_ptr<X11Window>& window, const char* title)
{
    printf("TODO()!\n");
}

#endif // JPLATFORM_LINUX
