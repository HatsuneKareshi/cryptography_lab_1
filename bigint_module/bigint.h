#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <random>

const int BIT_CNT_WRD = 64;
const int WORDCNT = 20; // change this to 20. default 512bit = 8
const char CHAR[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
const uint64_t LM_BIT_64 = 0x8000000000000000;
const uint64_t RM_BIT_64 = 0x0000000000000001;

// OF NOTE: THIS DOES NOT REALLY SUPPORT NEGATIVE NUMBERS

class bigint // needs to cover upto 512 bits
{
public:
    bool __bz_neg = false; // IMPORTANT
    // THIS NEGA FLAG IS ONLY FOR BEZOUT IDENTITY.
    // WILL NOT AFFECT EXISTING ARITHMETIC OPERATORS
    // DO NOT MODIFY ANYTHING STOCK. true here means NEGATIVE

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
    void div_mod(bigint divisor, bigint &quo, bigint &rmd);

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

    std::string debugstring(int n = WORDCNT);

    // stream things. NOTE: THESE BOTH OPERATE ON REVERSED STRINGS, AS PER
    // REQUIREMENTS
    friend std::ostream &operator<<(std::ostream &os, const bigint &num);
    friend std::istream &operator>>(std::istream &is, bigint &num);

    // __BEZOUT SPECIFIC HELPER METHODS:
    // the following are to access the __bz_neg attr.
    // only getters;
    // true here means NEGATIVE
    bool __bz_get_negflag();
    // true here means NEGATIVE
    void __bz_set_negflag(bool sgn);
};

bigint addMod(bigint a, bigint b, bigint n); // computes (a + b) % n

bigint mulMod(bigint a, bigint b, bigint n); // computes (a * b) % n

bigint powMod(bigint b, bigint x, bigint n); // computes (b ^ x) % n

bigint gcd(bigint a, bigint b); // this should be stein's algo

bigint random_bigint(bigint a, bigint b);

bool rabin_miller(bigint n, int iterations);

// between these 2 functions in the .cpp file exists
// bezout specific operators for plus, minus and multiplication
// only these functions interact with the __bz_neg flag

bigint __bz_add(bigint a, bigint b);

bigint __bz_sub(bigint a, bigint b); // a - b

bigint __bz_mul(bigint a, bigint b);

// PLEASE READ:
// this bezout function returns numbers unsigned, but they will display 2 comp negative.
// do NOT add them straight as ax + by, theres no guarentee that will work.
// they simply return the MAGNITUDE of x and y (2comp flipped if neg)
bigint bezout_ex_gcd(bigint a, bigint b, bigint &x, bigint &y, bool &neg_x, bool &neg_y); // returns x and y, NORMALIZED.

bool bezout_confirmation(bigint gcd, bigint a, bigint b, bigint x, bigint y, bool neg_x, bool neg_y);

void prettyprint_bigint(bigint a);