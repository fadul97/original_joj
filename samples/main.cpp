#include "joj/engine.h"
#include "app_gl.h"
#include <stdio.h>

#if JPLATFORM_WINDOWS
#include "app_dx11.h"
#endif

int main()
{
    joj::Engine* engine = joj::Engine::get_singleton();

    if JFAILED(engine->run(new DX11App(), joj::BackendRenderer::DX11))
        return -1;
    
    return 0;
}
