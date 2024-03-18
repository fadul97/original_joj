#ifndef JOJ_ENGINE_H
#define JOJ_ENGINE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error.h"
#include "app.h"

#if JPLATFORM_LINUX  
#include "platform/x11/platform_manager_x11.h"
#else
#include "platform/win32/platform_manager_win32.h"
#include "renderer/dx11/renderer_dx11.h"
#endif 

namespace joj
{
    // FIXME: Abstract default platform_manager and renderer
    class JAPI Engine
    {
    public:
        Engine();
        ~Engine();

        ErrorCode init();
        ErrorCode run(App* app);
        void shutdown();

        Win32PlatformManager* get_platform_manager() const;


#if JPLATFORM_WINDOWS  
        // TODO: static members?
        static Win32PlatformManager* platform_manager;
        static DX11Renderer* renderer;

        static void pause();	// Pause engine
        static void resume();	// Resume engine

        static void close();
        static b8 m_paused;
#endif // JPLATFORM_WINDOWS

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