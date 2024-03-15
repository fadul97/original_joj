#include "joj/engine.h"
#include "joj/app.h"

class MyApp : public joj::App
{
public:
    void init()
    {

    }

    void update(f32 dt)
    {

    }

    void draw()
    {
        joj::Engine::renderer->clear(1, 0, 0, 1);
    }

    void shutdown()
    {

    }
};

int main()
{
    joj::Engine* engine = new joj::Engine();
    if (engine->run(new MyApp()) != joj::ErrorCode::OK)
        return -1;

    return 0;
}
