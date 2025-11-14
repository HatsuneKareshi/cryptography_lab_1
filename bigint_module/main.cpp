#include "bigint.h"
#include <chrono>

// NOTE TO ALL: THIS main.cpp FILE IS NOT TO BE SUBMITTED AS THE FINAL PAYLOAD FOR ANY TASKS
//              THIS FILE IS ONLY TO TEST THE bigint CLASS

int main()
{
    bigint x, k, n;
    std::cout << "N, k, x?\n";
    std::cin >> n >> k >> x;
    std::cout << powMod(x, k, n);
}
