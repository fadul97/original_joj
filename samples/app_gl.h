#include "joj/engine.h"
#include "joj/app.h"
#include <stdio.h>

class GLApp : public joj::App
{
public:

    void init()
    {
       
    }

    void update(f32 dt)
    {
        if (joj::Engine::platform_manager->is_key_pressed(joj::KEY_ESCAPE))
            joj::Engine::close();
    }

    void draw()
    {
        joj::Engine::gl_renderer->clear(0, 1, 1, 1);
        SwapBuffers(joj::Engine::platform_manager->get_window()->get_device_context());
    }

    void shutdown()
    {
    }
};
