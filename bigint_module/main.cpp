#include "bigint.h"
#include <chrono>

// NOTE TO ALL: THIS main.cpp FILE IS NOT TO BE SUBMITTED AS THE FINAL PAYLOAD FOR ANY TASKS
//              THIS FILE IS ONLY TO TEST THE bigint CLASS

// now this is for question 4

int main()
{
    std::cout << "paste input file content here, question 4\n";
    bigint p, g, y, m, r, h;
    std::cin >> p >> g >> y >> m >> r >> h;
    bool chk1 = (bigint(0) < r && r < p) && (bigint(0) < h && h < p - bigint(1));

    bigint g_m = powMod(g, m, p);
    bigint y_r = powMod(y, r, p);
    bigint r_h = powMod(r, h, p);
    // prettyprint_bigint(g_m);
    // prettyprint_bigint(y_r);
    // prettyprint_bigint(r_h);
    // prettyprint_bigint(mulMod(y_r, r_h, p));

    bool chk2 = (g_m == mulMod(y_r, r_h, p));
    std::cout << (chk1 && chk2);
    return 0;
}
