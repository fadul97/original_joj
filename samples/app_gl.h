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
        if (joj::Engine::input->is_key_pressed(joj::KEY_ESCAPE))
            joj::Engine::window->close();
    }

    void draw()
    {
        joj::Engine::gl_renderer->clear(1, 1, 0, 0);
        joj::Engine::window->swap_buffers();
    }

    void shutdown()
    {
    }
};
