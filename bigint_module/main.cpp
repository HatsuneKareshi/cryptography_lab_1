#include "bigint.h"
#include <chrono>

// NOTE TO ALL: THIS main.cpp FILE IS NOT TO BE SUBMITTED AS THE FINAL PAYLOAD FOR ANY TASKS
//              THIS FILE IS ONLY TO TEST THE bigint CLASS

int main()
{
    bigint a;
    std::cin >> a;
    std::cout << a.debugstring() << "\n";
    std::cout << "should be reversed:\n";
    std::cout << a << "\n";
    if (rabin_miller(a, 5))
        std::cout << "prime" << "\n";
    else
        std::cout << "composite" << "\n";
}

// previous test below

// bigint a, b;
// a.words[7] = 1;
// b.words[7] = 3;
// auto start = std::chrono::high_resolution_clock::now();
// for (int i = 0; i < 100; i++)
// {
//     std::cout << random_bigint(a, b).debugstring() << "\n";
// }
// auto end = std::chrono::high_resolution_clock::now();
// auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
// std::cout << dur.count() << "\n";

// bigint a, b;
// a.words[7] = 0xD32754A36CE9127E;
// a.words[6] = 0x0AAA74F04B857C88;
// a.words[5] = 0x84EA50768F191229;
// a.words[4] = 0x16070C4FDC0013C0;

// b.words[7] = 0x2FF30576A6A0D0D7;

// a.words[7] = 0x0000000000000014;
// b.words[7] = 0x0000000000000014;

// std::cout << "a / b\t" << (a % b).debugstring() << "\n";
// std::cout << "a / b\t" << (a % b).debugstring() << "\n";

// std::cout << (b > a) << "\n"
//           << (b < a) << "\n"
//           << (b >= a) << "\n"
//           << (b <= a) << "\n";

// bigint a, b;
// a.words[7] = 0x0000000000F3A53D;
// b.words[7] = 0x0000000000000156;
// std::cout << (a / b).debugstring() << "\n";
