#include "joj/engine.h"

int main()
{
    joj::Engine* engine = new joj::Engine();
    if (engine->run() != joj::ErrorCode::OK)
        return -1;

    return 0;
}
