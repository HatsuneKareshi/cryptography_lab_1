#include "bigint.h"
#include <chrono>

// NOTE TO ALL: THIS main.cpp FILE IS NOT TO BE SUBMITTED AS THE FINAL PAYLOAD FOR ANY TASKS
//              THIS FILE IS ONLY TO TEST THE bigint CLASS

int main()
{
    std::cout << "paste input file content here, question 2\n";
    bigint p, g, a, b; // A for alex, B for bob;
    std::cin >> p >> g >> a >> b;
    std::cout << "output:\n";
    std::cout << powMod(g, a, p) << "\n"
              << powMod(g, b, p) << "\n"
              << powMod(powMod(g, a, p), b, p) << "\n";
    return 0;
}
