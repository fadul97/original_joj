#include <iostream>

#include "joj/joj.h"
#include "joj/defines.h"

int main()
{
#if JPLATFORM_LINUX
    std::cout << "LINUX!\n";
#endif

    std::cout << "Hello, Joj!\n";
    joj_print();

    u8 a = 0;
    std::cout << "a = " << a << "\n";

    u32 b = 10;
    std::cout << "b = " << b << "\n";

    i32 c = -10;
    std::cout << "c = " << c << "\n";
    return 0;
}
