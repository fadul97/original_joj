#ifndef JOJ_ENGINE_H
#define JOJ_ENGINE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <vector>
#include "error.h"
#include "renderer/renderer.h"

#if JPLATFORM_WINDOWS
#include "platform/win32/platform_manager_win32.h"
#include "renderer/dx11/renderer_dx11.h"
#include "renderer/opengl/renderer_gl.h"
#else
#include "platform/x11/platform_manager_x11.h"
#include "renderer/opengl/renderer_gl.h"
#endif

namespace joj
{
#if JPLATFORM_WINDOWS
    using JPlatformManager = Win32PlatformManager;
    using JRenderer = DX11Renderer;
    using JGLRenderer = GLRenderer;
    using JWindow = Win32Window;
#else
    using JojPlatformManager = X11PlatformManager;
    using JRenderer = GLRenderer;
    using JWindow = X11Window;
#endif
}

namespace joj
{
    class App;
    
    // FIXME: Abstract default platform_manager and renderer
    class JAPI Engine
    {
    public:
        Engine();
        ~Engine();

        ErrorCode init(RendererBackend renderer_backend);
        ErrorCode run(App* app, RendererBackend renderer_backend);
        void shutdown();

        JPlatformManager* get_platform_manager() const;

        // TODO: static members?
        static JPlatformManager* platform_manager;
        static JRenderer* renderer;
        static JGLRenderer* gl_renderer;
        static std::vector<Error> errors;

        void ouput_log() const;

        static void pause();	// Pause engine
        static void resume();	// Resume engine

        static void close();
        static b8 m_paused;


    private:
        f32 get_frametime();
        f32 m_frametime;
    };

    inline void Engine::pause()
	{ m_paused = true; platform_manager->stop_timer(); }

	inline void Engine::resume()
	{ m_paused = false; platform_manager->start_timer(); }

    inline void Engine::close()
    { platform_manager->close_window(); }

} // namespace joj

#ifndef JADDERROR
#define JADDERROR(x)                                                        \
{                                                                           \
    joj::ErrorCode ecode = (x);                                             \
    joj::Error new_error = joj::Error(ecode, __func__, __FILE__, __LINE__); \
    joj::Engine::errors.push_back(new_error);                               \
}
#endif

#endif // JOJ_ENGINE_H
