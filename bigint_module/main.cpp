#include "bigint.h"
#include <chrono>

// NOTE TO ALL: THIS main.cpp FILE IS NOT TO BE SUBMITTED AS THE FINAL PAYLOAD FOR ANY TASKS
//              THIS FILE IS ONLY TO TEST THE bigint CLASS

int main()
{

    bigint a;
    a.words[7] = RM_BIT_64;
    a.words[4] = RM_BIT_64;
    std::cout << a.debugstring() << "\n";
    std::cout << a << "\n";
    // bool _xneg, _yneg;
    // bigint p, q, e, d, _;
    // bigint phi;
    // std::cout << "p, q, e?\n";
    // std::cin >> p >> q >> e;
    // phi = (p - bigint(1)) * (q - bigint(1));
    // if (gcd(e, phi) != bigint(1))
    //     std::cout << "NO\n";
    // else
    // {
    //     bezout_ex_gcd(e, phi, d, _, _xneg, _yneg);
    //     std::cout << "decryption key:"
    //               << d.debugstring(8) << "\n"
    //               << d << "\n";
    // }
    // return 0;
}

// previous test below
// std::cout << "signs for both\n";
// std::cin >> sgna >> sgnb;
// bigint a(ia), b(ib);
// a.__bz_set_negflag(sgna);
// b.__bz_set_negflag(sgnb);
// std::cout << "a add b = ";
// prettyprint_bigint(__bz_add(a, b));
// std::cout << "a sub b = ";
// prettyprint_bigint(__bz_sub(a, b));
// std::cout << "a mul b = ";
// prettyprint_bigint(__bz_mul(a, b));

// std::cout << "=======\nb add a = ";
// prettyprint_bigint(__bz_add(b, a));
// std::cout << "b sub a = ";
// prettyprint_bigint(__bz_sub(b, a));
// std::cout << "b mul a = ";
// prettyprint_bigint(__bz_mul(b, a));

// uint64_t ia, ib, sgna, sgnb;
// bool negx, negy;
// std::cout << "2 NUMBERS\n";
// std::cin >> ia >> ib;
// bigint a(ia), b(ib), x, y; // ax + by = gcd(a, b)
// bigint gcdiddy = bezout_ex_gcd(a, b, x, y, negx, negy);
// std::cout << "a = ";
// prettyprint_bigint(a);
// std::cout << "b = ";
// prettyprint_bigint(b);
// std::cout << "gcd = ";
// prettyprint_bigint(gcdiddy);
// std::cout << "x = ";
// prettyprint_bigint(x);
// std::cout << "y = ";
// prettyprint_bigint(y);

// std::cout << "bezout test status: " << bezout_confirmation(gcdiddy, a, b, x, y, negx, negy);