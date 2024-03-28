#include "joj/engine.h"
#include "app_gl.h"
#include <stdio.h>

#if JPLATFORM_WINDOWS
#include "app_dx11.h"
#endif

int main()
{
    joj::Engine* engine = joj::Engine::get_singleton();

    if JFAILED(engine->run(new GLApp(), joj::BackendRenderer::GL))
        return -1;
    
    return 0;
}
