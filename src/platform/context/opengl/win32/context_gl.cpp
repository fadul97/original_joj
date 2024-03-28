#include "platform/context/opengl/win32/context_gl.h"

#if JPLATFORM_WINDOWS

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"
#include <string.h>

typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

joj::Win32GLContext::Win32GLContext()
{
	m_rc = nullptr;
    m_color_bits = 32;
    m_depth_bits = 24;

    // Support for OpenGL rendering.
    m_pixel_format_attrib_list[0] = WGL_SUPPORT_OPENGL_ARB;
    m_pixel_format_attrib_list[1] = TRUE;

    // Support for rendering to a window.
    m_pixel_format_attrib_list[2] = WGL_DRAW_TO_WINDOW_ARB;
    m_pixel_format_attrib_list[3] = TRUE;

    // Support for hardware acceleration.
    m_pixel_format_attrib_list[4] = WGL_ACCELERATION_ARB;
    m_pixel_format_attrib_list[5] = WGL_FULL_ACCELERATION_ARB;

    // Support for 32bit color.
    m_pixel_format_attrib_list[6] = WGL_COLOR_BITS_ARB;
    m_pixel_format_attrib_list[7] = m_color_bits;

    // Support for 24 bit depth buffer.
    m_pixel_format_attrib_list[8] = WGL_DEPTH_BITS_ARB;
    m_pixel_format_attrib_list[9] = m_depth_bits;

    // Support for double buffer.
    m_pixel_format_attrib_list[10] = WGL_DOUBLE_BUFFER_ARB;
    m_pixel_format_attrib_list[11] = TRUE;

    // Support for swapping front and back buffer.
    m_pixel_format_attrib_list[12] = WGL_SWAP_METHOD_ARB;
    m_pixel_format_attrib_list[13] = WGL_SWAP_EXCHANGE_ARB;

    // Support for the RGBA pixel type.
    m_pixel_format_attrib_list[14] = WGL_PIXEL_TYPE_ARB;
    m_pixel_format_attrib_list[15] = WGL_TYPE_RGBA_ARB;

    // Support for a 8 bit stencil buffer.
    m_pixel_format_attrib_list[16] = WGL_STENCIL_BITS_ARB;
    m_pixel_format_attrib_list[17] = 8;

    // Null terminate the attribute list.
    m_pixel_format_attrib_list[18] = 0;

    m_gl_version_major = 4;
    m_gl_version_minor = 6;

    const i32 ctx_attribs[] =
    {
      WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<i32>(m_gl_version_major),
      WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<i32>(m_gl_version_minor),
      WGL_CONTEXT_FLAGS_ARB,
#ifdef _DEBUG
      WGL_CONTEXT_DEBUG_BIT_ARB |
#endif // _DEBUG
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0
    };

    memcpy(m_context_attribs, ctx_attribs, sizeof(ctx_attribs));
}

joj::Win32GLContext::~Win32GLContext()
{
}

b8 joj::Win32GLContext::create(std::unique_ptr<Win32Window>& window)
{
    auto dummy_window = new joj::Win32Window();
    dummy_window->create();

    // TODO: Add comments about pfd
    // Describe pixel format
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        (BYTE)m_depth_bits,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        (BYTE)m_color_bits,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    u32 pixel_format = ChoosePixelFormat(dummy_window->get_device_context(), &pfd);
    if (!pixel_format)
    {
        // TODO: Use own logger and return value
        printf("Failed to ChoosePixelFormat for dummy_window.\n");
        DestroyWindow(dummy_window->get_id());
        return false;
    }

    if (!SetPixelFormat(dummy_window->get_device_context(), pixel_format, &pfd))
    {
        // TODO: Use own logger and return value
        printf("Failed to SetPixelFormat for dummy_window.\n");
        DestroyWindow(dummy_window->get_id());
        return false;
    }

    HGLRC new_rc = wglCreateContext(dummy_window->get_device_context());
    if (!new_rc)
    {
        // TODO: Use own logger and return value
        printf("Failed to wglCreateContext for dummy_window.\n");
        DestroyWindow(dummy_window->get_id());
        return false;
    }

    if (!wglMakeCurrent(dummy_window->get_device_context(), new_rc))
    {
        // TODO: Use own logger and return value
        printf("Failed to wglMakeCurrent for dummy_window.\n");
        DestroyWindow(dummy_window->get_id());
        return false;
    }

    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    if (!wglChoosePixelFormatARB)
    {
        // TODO: Use own logger and return value
        printf("Failed to wglGetProcAddress of wglChoosePixelFormatARB.\n\n");
        return false;
    }

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB)
    {
        // TODO: Use own logger and return value
        printf("Failed to wglGetProcAddress of wglCreateContextAttribsARB.\n\n");
        return false;
    }

    wglMakeCurrent(0, 0);
    wglDeleteContext(new_rc);
    DestroyWindow(dummy_window->get_id());

    int new_pixel_format;
    int num_pixel_formats = 0;
    PIXELFORMATDESCRIPTOR new_pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        (BYTE)m_depth_bits,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        (BYTE)m_color_bits,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    const int* pxf_attrib_list = (const int*)m_pixel_format_attrib_list;
    const int* context_attrib_list = (const int*)m_context_attribs;

    wglChoosePixelFormatARB(window->get_device_context(), pxf_attrib_list, nullptr, 1, &new_pixel_format, (UINT*)&num_pixel_formats);
    if (num_pixel_formats <= 0)
    {
        // TODO: Use own logger and return value
        printf("Failed to wglChoosePixelFormatARB.\n");
        return false;
    }

    if (!SetPixelFormat(window->get_device_context(), new_pixel_format, &pfd))
    {
        // TODO: Use own logger and return value
        printf("Failed to SetPixelFormat.\n");
        return false;
    }

    new_rc = wglCreateContextAttribsARB(window->get_device_context(), 0, context_attrib_list);
    if (!new_rc)
    {
        // TODO: Use own logger and return value
        printf("Failed to wglCreateContextAttribsARB.\n");
        return false;
    }

    m_rc = new_rc;

    if (!wglMakeCurrent(window->get_device_context(), m_rc))
    {
        // TODO: Use own logger and return value
        printf("Failed to wglMakeCurrent of window.\n");
        return false;
    }

    return true;
}

// FIXME: make_current method only loads opengl functions (mainly)
void joj::Win32GLContext::make_current(std::unique_ptr<Win32Window>& window)
{
    load_opengl_functions();

    COLORREF c = window->get_color();

    f32 r = GetRValue(window->get_color()) / 255.0f;
    f32 g = GetGValue(window->get_color()) / 255.0f;
    f32 b = GetBValue(window->get_color()) / 255.0f;

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SwapBuffers(window->get_device_context());

#if _DEBUG
    log_hardware_info();
#endif // _DEBUG
}

void joj::Win32GLContext::log_hardware_info()
{
    // TODO: Use own logger
    printf("OpenGL Version: %s.\n", glGetString(GL_VERSION));
    printf("OpenGL Renderer: %s.\n", glGetString(GL_RENDERER));
    printf("OpenGL GLSL Version: %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void joj::Win32GLContext::destroy()
{
    printf("TODO()!\n");    
}

#endif // JPLATFORM_WINDOWS
