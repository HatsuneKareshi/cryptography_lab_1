#include "bigint.h"
#include <chrono>
#include <vector> // yes this is allowed;

// NOTE TO ALL: THIS main.cpp FILE IS NOT TO BE SUBMITTED AS THE FINAL PAYLOAD FOR ANY TASKS
//              THIS FILE IS ONLY TO TEST THE bigint CLASS

int main()
{
    std::cout << "paste input file content here\n";
    bigint p, g, n;
    std::vector<bigint> prime_factors{};
    std::cin >> p;
    std::cin >> n;
    for (bigint i(0); i < n; i += 1)
    {
        bigint tmp_prime;
        std::cin >> tmp_prime;
        prime_factors.push_back(tmp_prime);
    }
    std::cin >> g;

    bool answer = true;

    for (auto pf : prime_factors)
    {
        if (powMod(g, (p - bigint(1)) / pf, p) == 1)
        {
            answer = false;
            break;
        }
    }
    std::cout << ((answer) ? "yes\n1" : "no\n0") << "\n";
    return 0;
}
