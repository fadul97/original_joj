#include "joj/engine.h"
#include "app_gl.h"
#include <stdio.h>

#if JPLATFORM_WINDOWS
#include "app_dx11.h"
#endif

int main()
{
    joj::Engine* engine = new joj::Engine();

    if JFAILED(engine->run(new DX11App(), joj::RendererBackend::DX11))
        return -1;
    
    delete engine;
    return 0;
}
