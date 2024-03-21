#include "joj/engine.h"
#include "app_gl.h"

int main()
{
    joj::Engine* engine = new joj::Engine();
    if (engine->run(new MyApp()) != joj::ErrorCode::OK)
        return -1;

    delete engine;
    return 0;
}
