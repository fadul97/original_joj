#ifndef JOJ_ENGINE_H
#define JOJ_ENGINE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <vector>
#include "error.h"
#include "renderer/renderer.h"
#include <memory>

#if JPLATFORM_WINDOWS
#include "platform/win32/platform_manager_win32.h"
#include "platform/win32/window_win32.h"
#include "platform/win32/input_win32.h"
#include "platform/win32/timer_win32.h"
#include "renderer/dx11/renderer_dx11.h"
#include "renderer/opengl/renderer_gl.h"
#else
#include "platform/x11/platform_manager_x11.h"
#include "platform/x11/window_x11.h"
#include "platform/x11/input_x11.h"
#include "renderer/opengl/renderer_gl.h"
#endif

// FIXME: Abstract classes
namespace joj
{
#if JPLATFORM_WINDOWS
    using JPlatformManager = Win32PlatformManager;
    using JRenderer = DX11Renderer;
    using JGLRenderer = GLRenderer;
#else
    using JojPlatformManager = X11PlatformManager;
    using JRenderer = GLRenderer;
#endif
}

namespace joj
{
    class App;
    
    // FIXME: Abstract default platform_manager and renderer
    class JAPI Engine
    {
    public:
        ErrorCode init(BackendRenderer backend_renderer);
        ErrorCode run(App* app, BackendRenderer backend_renderer);
        void shutdown();

        // TODO: static members?
        static  std::unique_ptr<JPlatformManager> platform_manager;
        static std::unique_ptr<JRenderer> dx11_renderer;
        static std::unique_ptr<JGLRenderer> gl_renderer;
        static std::vector<Error> errors;

        void ouput_log() const;

        static void pause();	// Pause engine
        static void resume();	// Resume engine
        
        static void close();
        static b8 m_paused;

        static Engine* get_singleton();

#if JPLATFORM_WINDOWS
        static LRESULT CALLBACK EngineProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // JPLATFORM_WINDOWS

    private:
        Engine();
        ~Engine();

        static Engine* engine;
        f32 get_frametime();
        f32 m_frametime;
        static b8 m_running;
        static App* m_app;
    };

    inline Engine* Engine::get_singleton()
    { 
        if (engine == nullptr)
            engine = new Engine();

        return engine;
    }

    inline void Engine::pause()
	{ m_paused = true; platform_manager->stop_timer(); }

	inline void Engine::resume()
	{ m_paused = false; platform_manager->start_timer(); }

    inline void Engine::close()
    { m_running = false; platform_manager->close_window(); }
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
