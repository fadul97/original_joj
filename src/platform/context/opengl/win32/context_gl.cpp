#include "platform/context/opengl/win32/context_gl.h"

#if JPLATFORM_WINDOWS

#define JOJ_GL_DEFINE_EXTERN
#include "platform/context/opengl/joj_gl.h"

#include <logger.h>

#include <windows.h>
#include <strsafe.h>

typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

joj::Win32GLContext::Win32GLContext() {
    m_context_config = GLContextConfig {
        .shared_context = nullptr,
    };

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
    if (m_context_config.shared_context != nullptr) {
        wglDeleteContext(m_context_config.shared_context);
    }
}

joj::ErrorCode joj::Win32GLContext::create(WindowConfig &window) {
    Win32Window dummy_window{};
    dummy_window.create(0, 0, "", WindowMode::Borderless);

    const PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        static_cast<BYTE>(m_depth_bits),
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        static_cast<BYTE>(m_color_bits),
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    dummy_window.get_window_config().hdc = GetDC(nullptr);

    const i32 pixel_format = ChoosePixelFormat(dummy_window.get_window_config().hdc, &pfd);
    if (!pixel_format)
    {
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to ChoosePixelFormat for dummy_window.");
        dummy_window.destroy();
        return ErrorCode::ERR_CONTEXT;
    }

    if (!SetPixelFormat(dummy_window.get_window_config().hdc, pixel_format, &pfd))
    {
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to SetPixelFormat for dummy_window.");
        dummy_window.destroy();
        return ErrorCode::ERR_CONTEXT;
    }

    HGLRC new_rc = wglCreateContext(dummy_window.get_window_config().hdc);
    if (!new_rc)
    {
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to wglCreateContext for dummy_window.");
        dummy_window.destroy();
        return ErrorCode::ERR_CONTEXT;
    }

    if (!wglMakeCurrent(dummy_window.get_window_config().hdc, new_rc))
    {
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to wglMakeCurrent for dummy_window.");
        dummy_window.destroy();
        return ErrorCode::ERR_CONTEXT;
    }

    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    if (!wglChoosePixelFormatARB)
    {
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to wglGetProcAddress of wglChoosePixelFormatARB.");
        dummy_window.destroy();
        return ErrorCode::ERR_CONTEXT;
    }

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB)
    {
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to wglGetProcAddress of wglCreateContextAttribsARB.");
        dummy_window.destroy();
        return ErrorCode::ERR_CONTEXT;
    }

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(new_rc);
    dummy_window.destroy();

    i32 new_pixel_format;
    i32 num_pixel_formats = 0;
    const PIXELFORMATDESCRIPTOR new_pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        static_cast<BYTE>(m_depth_bits),
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        static_cast<BYTE>(m_color_bits),
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    const i32* pxf_attrib_list = m_pixel_format_attrib_list;
    const i32* context_attrib_list = m_context_attribs;

    // window.hdc = GetDC(nullptr);

    wglChoosePixelFormatARB(window.hdc, pxf_attrib_list, nullptr, 1, &new_pixel_format, (UINT*)&num_pixel_formats);
    if (num_pixel_formats <= 0)
    {
        // TODO: Use better return values
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to wglChoosePixelFormatARB.");
        return ErrorCode::ERR_CONTEXT;
    }

    // window.hdc = GetDC(window.handle);
    // window.hdc = GetDC(nullptr);

    if (!SetPixelFormat(window.hdc, new_pixel_format, &new_pfd))
    {
        // Retrieve the system error message for the last-error code

        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;
        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );

        // Display the error message and exit the process

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)SetPixelFormat) + 40) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lpDisplayBuf,
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"),
            SetPixelFormat, dw, lpMsgBuf);
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
        ExitProcess(dw);

        // TODO: Use better return values
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to SetPixelFormat.");
        return ErrorCode::ERR_CONTEXT;
    }

    m_context_config.shared_context = wglCreateContextAttribsARB(window.hdc, nullptr, context_attrib_list);
    if (!m_context_config.shared_context)
    {
        // TODO: Use better return values
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to wglCreateContextAttribsARB.");
        return ErrorCode::ERR_CONTEXT;
    }

    return ErrorCode::OK;
}

// FIXME: make_current method only loads opengl functions (mainly)
joj::ErrorCode joj::Win32GLContext::make_current(WindowConfig &window) {
    if (!wglMakeCurrent(window.hdc, m_context_config.shared_context))
    {
        // TODO: Use better return values
        JFATAL(ErrorCode::ERR_CONTEXT, "Failed to wglMakeCurrent of window.");
        return ErrorCode::ERR_CONTEXT;
    }

    load_opengl_functions();

    log_hardware_info();

    glClearColor(1, 0, 1, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SwapBuffers(window.hdc);

    return ErrorCode::OK;
}

void joj::Win32GLContext::destroy()
{
    if (m_context_config.shared_context != nullptr) {
        wglDeleteContext(m_context_config.shared_context);
    }
}


void joj::Win32GLContext::log_hardware_info()
{
    if (glGetString) {
        JDEBUG("OpenGL Version: %s.", glGetString(GL_VERSION));
        JDEBUG("OpenGL Renderer: %s.", glGetString(GL_RENDERER));
        JDEBUG("OpenGL GLSL Version: %s.", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }
}

#endif // JPLATFORM_WINDOWS
