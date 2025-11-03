#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <random>

const int BIT_CNT_WRD = 64;
const int WORDCNT = 8;
const char CHAR[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const uint64_t LM_BIT_64 = 0x8000000000000000;
const uint64_t RM_BIT_64 = 0x0000000000000001;

class bigint // needs to cover upto 512 bits
{
public:
    uint64_t words[WORDCNT]; // 0th index is the MOST significant

    bigint();
    bigint(const bigint &a);
    bigint &operator=(const bigint &a);
    bigint(std::string str); // takes reversed strings as per requirements specs

    bigint operator+(const bigint &a) const;
    bigint operator-(const bigint &a) const;
    bigint operator<<(int times) const; // left shift
    bigint operator>>(int times) const; // right shift
    bigint operator*(const bigint &a) const;
    bigint operator/(const bigint &a) const; // (soon) now
    bigint operator%(const bigint &a) const;

    bigint get_twocomp_neg();

    bool operator==(const bigint &a);
    bool operator!=(const bigint &a);
    bool operator>(const bigint &a);
    bool operator<(const bigint &a);
    bool operator>=(const bigint &a);
    bool operator<=(const bigint &a); //

    bool get_bit(int bit) const;       // 0th bit is the LEAST significant
    void set_bit(int bit, bool value); // same up top

    // quality of life BS

    bigint(uint64_t cst);
    bool is_even();
    bigint &operator+=(const bigint &a);
    bigint &operator-=(const bigint &a);
    bigint &operator*=(const bigint &a);
    bigint &operator/=(const bigint &a);
    bigint &operator%=(const bigint &a);

    std::string debugstring();
};

bigint addMod(bigint a, bigint b, bigint n); // computes (a + b) % n

bigint mulMod(bigint a, bigint b, bigint n); // computes (a * b) % n

bigint powMod(bigint b, bigint x, bigint n); // computes (b ^ x) % n

bigint gcd(bigint a, bigint b); // this should be stein's algo

bigint random_bigint(bigint a, bigint b);

bool rabin_miller(bigint n, int iterations);