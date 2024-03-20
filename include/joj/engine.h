#ifndef JOJ_ENGINE_H
#define JOJ_ENGINE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error.h"

#if JPLATFORM_WINDOWS
#include "platform/win32/platform_manager_win32.h"
#include "renderer/dx11/renderer_dx11.h"
#else
#include "platform/x11/platform_manager_x11.h"
#include "renderer/opengl/renderer_gl.h"
#endif

namespace joj
{
#if JPLATFORM_WINDOWS
    using JojPlatformManager = Win32PlatformManager;
    using JojRenderer = DX11Renderer;
#else
    using JojPlatformManager = X11PlatformManager;
    using JojRenderer = GLRenderer;
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

        ErrorCode init();
        ErrorCode run(App* app);
        void shutdown();

        JojPlatformManager* get_platform_manager() const;

        // TODO: static members?
        static JojPlatformManager* platform_manager;
        static JojRenderer* renderer;

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

#endif // JOJ_ENGINE_H
