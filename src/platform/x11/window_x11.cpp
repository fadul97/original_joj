#include "platform/x11/window_x11.h"

#if JPLATFORM_LINUX

#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>

joj::X11Window::X11Window(i16 width, i16 height, std::string title)
    : joj::Window<u32>(width, height, title)
{
    display = nullptr;
    id = 0;
    screen = nullptr;
    wnd_attribs = { 0 };
    visual = nullptr;
    delete_msg = 0;
}

joj::X11Window::~X11Window()
{
}

void joj::X11Window::hide_cursor(b8 hide)
{
    printf("TODO()!\n");
}

b8 joj::X11Window::init()
{
    // Open display
    display = XOpenDisplay(NULL);
    if (display == nullptr)
    {
        // TODO: use own logger and return value
        printf("Failed to open display.\n");
        return false;
    }


    // Setup screen
    screen = DefaultScreenOfDisplay(display);
    screen_id = DefaultScreen(display);

    return true;
}

b8 joj::X11Window::create()
{
    // Set window attributes
    wnd_attribs.border_pixel = BlackPixel(display, screen_id);
    wnd_attribs.background_pixel = WhitePixel(display, screen_id);
    wnd_attribs.override_redirect = True;
    wnd_attribs.colormap = XCreateColormap(
        display, 
        RootWindow(display, screen_id),
        visual->visual,
        AllocNone
    );
    wnd_attribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask
        | KeymapStateMask   | StructureNotifyMask   | ButtonPressMask 
        | ButtonReleaseMask | EnterWindowMask       | LeaveWindowMask 
        | PointerMotionMask | FocusChangeMask;
    
    // Create window
    id = XCreateWindow(
        display,
        RootWindow(display, screen_id),
        0, 0,
        width, height,
        0,
        visual->depth,
        InputOutput,
        visual->visual,
        CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
        &wnd_attribs
    );

    if (id == 0)
    {
        // TODO: use own logger and return value
        printf("Failed to create window.\n");
        XCloseDisplay(display);
        return false;
    }

    // Set window title
    XStoreName(display, id, title.c_str());
    
    // Redirect close
    delete_msg = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, id, &delete_msg, 1);

    return true;
}

void joj::X11Window::show()
{
    // Show window
    XClearWindow(display, id);
    XMapRaised(display, id);

    // Sync to update window info
    XSync(display, id);

    running = true;
}

void joj::X11Window::clear()
{
    printf("TODO()!\n");
}

void joj::X11Window::swap_buffers()
{
    // glXSwapBuffers();
    printf("TODO()!\n");
}

void joj::X11Window::shutdown()
{
    // Release id
    if (id)
        XDestroyWindow(display, id);

    // Release display
    if (display)
        XCloseDisplay(display);
}


b8 joj::X11Window::create_simple_window()
{
    // Open display
    display = XOpenDisplay(NULL);
    if(display == NULL)
    {
        // TODO: use own logger and return value
        printf("Failed to open display.\n");
        return false;
    }

    screen_id = DefaultScreen(display);

    unsigned long black = BlackPixel(display, 0);
    unsigned long white = WhitePixel(display, 0);

    id = XCreateSimpleWindow(
        display,
        DefaultRootWindow(display),
        0, 0,
        width, height,
        5,
        white,
        black);

    if(id == 0)
    {
        // TODO: use own logger and return value
        printf("Failed to create window.\n");
        XCloseDisplay(display);
        return false;
    }

    this->width = width;
    this->height = height;
    this->title = title;

    XStoreName(display, id, title.c_str());

    XSelectInput(display, id, ExposureMask 
        | KeyPressMask          | KeyReleaseMask    | KeymapStateMask
        | StructureNotifyMask   | ButtonPressMask   | ButtonReleaseMask
        | EnterWindowMask       | LeaveWindowMask   | PointerMotionMask
        | FocusChangeMask);

    gc = XCreateGC(display, id, 0, 0);

    XSetBackground(display, gc, white);
    XSetForeground(display, gc, white);

    // Redirect close
    delete_msg = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, id, &delete_msg, 1);

    return true;
}

#endif // JPLATFORM_LINUX
