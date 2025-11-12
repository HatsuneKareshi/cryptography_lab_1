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

    std::string debugstring(int n = 8);

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

///////////////////////////////////////////////////////
/////// bigint.h //////////////////////////////////////
///////////////////////////////////////////////////////

std::string __ui64_to_hex_str(uint64_t a)
{
    std::string result{};
    for (int i = 0; i < 16; i++)
    {
        result.push_back(CHAR[a % 16]);
        a /= 16;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string __ui64_to_hex_unpadded(uint64_t a)
{
    std::string result{};
    for (; a > 0;)
    {
        result.push_back(CHAR[a % 16]);
        a /= 16;
    }
    std::reverse(result.begin(), result.end());
    return result;
} /// no

//// auxillary, helpers ////
////////////////////////////
//// class specifics    ////

bigint::bigint()
{
    for (int i = 0; i < WORDCNT; i++)
        this->words[i] = 0; // sets everything to 0, even if uninit'd
}

bigint::bigint(const bigint &a)
{
    for (int i = 0; i < WORDCNT; i++)
        this->words[i] = a.words[i];
    this->__bz_neg = a.__bz_neg; // makes sure i can actually return the damn sign correctly
}

bigint &bigint::operator=(const bigint &a)
{
    for (int i = 0; i < WORDCNT; i++)
        this->words[i] = a.words[i];
    this->__bz_neg = a.__bz_neg; // makes sure i can actually return the damn sign correctly
    return *this;
}

bigint::bigint(std::string a)
{
    for (int i = 0; i < WORDCNT; i++)
        this->words[i] = 0; // sets everything to 0, even if uninit'd

    int len = a.length();
    for (int i = len - 1; i >= 0; i--)
    {
        *this = (*this << 4); // multiplied by 16
        switch (a[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            this->words[WORDCNT - 1] += uint64_t(a[i] - '0');
        }
        break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        {
            this->words[WORDCNT - 1] += uint64_t(a[i] - 'A' + 10);
        }
        break;
        }
    }
}

bigint bigint::operator+(const bigint &a) const
{
    bool _carrier = false;
    bigint result;
    for (int i = WORDCNT - 1; i >= 0; i--)
    {
        uint64_t temp = this->words[i] + a.words[i] + _carrier;
        result.words[i] = temp;
        if (temp < this->words[i] || temp < a.words[i])
            _carrier = true; // uint64_t should be unsigned. if it suddenly becomes smaller then either that should mean overflow -> carry
        else
            _carrier = false;
    }
    return result;
}

bigint bigint::operator-(const bigint &a) const
{
    bigint neg_b = a;
    neg_b = neg_b.get_twocomp_neg();
    return *this + neg_b;
}

bigint bigint::operator<<(int times) const
{
    bigint answer = *this;
    for (int i = 0; i < times; i++)
    {
        bool left_drop = false;
        for (int j = WORDCNT - 1; j >= 0; j--)
        {
            // answer.words[j] |= left_drop;
            bool state = left_drop;
            left_drop = LM_BIT_64 & answer.words[j]; // 1000 & x. 1 if one, 0 if zero
            answer.words[j] = (answer.words[j] << 1);
            answer.words[j] |= state;
        }
    }
    return answer;
}

bigint bigint::operator>>(int times) const
{
    bigint answer = *this;
    for (int i = 0; i < times; i++)
    {
        bool right_drop = false;
        for (int j = 0; j < WORDCNT; j++)
        {
            bool state = right_drop;
            right_drop = RM_BIT_64 & answer.words[j];
            answer.words[j] = (answer.words[j] >> 1);
            if (state)
                answer.words[j] |= LM_BIT_64;
        }
    }
    return answer;
}

bigint bigint::operator*(const bigint &a) const
{
    bigint answer;
    bigint addant = *this;
    for (int i = 0; i < WORDCNT * BIT_CNT_WRD; i++)
    {
        if (a.get_bit(i))
            answer = answer + addant;
        addant = addant << 1;
    }
    return answer;
}

bigint bigint::operator/(const bigint &a) const
{
    // bigint answer;
    // bigint divident_dummy = *this;
    // while (divident_dummy >= a)
    // {
    //     bigint divisor_dummy = a;
    //     uint64_t times = 0;
    //     while (divident_dummy >= divisor_dummy)
    //     {
    //         divisor_dummy = divisor_dummy << 1;
    //         times++;
    //     }
    //     bigint one;
    //     one.words[WORDCNT - 1] = RM_BIT_64;
    //     answer = answer + ((one << times) >> 1);
    //     divident_dummy = divident_dummy - (divisor_dummy >> 1);
    // }
    // return answer;
    bigint answer; // init to 0;
    bigint divident = *this;
    bigint divisor = a;
    bigint one;
    bigint zero;
    one.words[WORDCNT - 1] = RM_BIT_64;
    if (divisor == zero)
        throw std::invalid_argument("division by 0 not");
    for (int i = BIT_CNT_WRD * WORDCNT - 1; i >= 0; i--)
    {
        if ((divisor << i) == zero) // hopefully detects overflow
            continue;
        else if ((divisor << i) <= divident)
        {
            divident = divident - (divisor << i);
            answer = answer + (one << i);
        }
    }
    return answer;
} // division by 0, be careful!!!

bigint bigint::operator%(const bigint &a) const
{
    bigint quotient; // init to 0;
    bigint divident = *this;
    bigint divisor = a;
    bigint one;
    bigint zero;
    one.words[WORDCNT - 1] = RM_BIT_64;
    if (divisor == zero)
        throw std::invalid_argument("division by 0 not");
    for (int i = BIT_CNT_WRD * WORDCNT - 1; i >= 0; i--)
    {
        if ((divisor << i) == zero) // hopefully detects overflow
            continue;
        else if ((divisor << i) <= divident)
        {
            divident = divident - (divisor << i);
            quotient = quotient + (one << i);
        }
    }
    return divident; // return whatever is left of divident. makes no changes to this.
}

void bigint::div_mod(bigint dvsor, bigint &quo, bigint &rmd)
{
    bigint quotient; // init to 0;
    bigint divident = *this;
    bigint divisor = dvsor;
    bigint one;
    bigint zero;
    one.words[WORDCNT - 1] = RM_BIT_64;
    if (divisor == zero)
        throw std::invalid_argument("division by 0 not");
    for (int i = BIT_CNT_WRD * WORDCNT - 1; i >= 0; i--)
    {
        if ((divisor << i) == zero) // hopefully detects overflow
            continue;
        else if ((divisor << i) <= divident)
        {
            divident = divident - (divisor << i);
            quotient = quotient + (one << i);
        }
    }
    quo = quotient; // quotient.
    rmd = divident; // what is left of the divident is the remainder
}

bigint bigint::get_twocomp_neg()
{
    bigint result = *this;
    for (int i = 0; i < WORDCNT; i++)
        result.words[i] = ~result.words[i]; // flip all of em

    bigint the_one;
    the_one.words[WORDCNT - 1] = RM_BIT_64;
    return result + the_one;
}

bool bigint::operator==(const bigint &a)
{
    for (int i = 0; i < WORDCNT; i++)
        if (this->words[i] != a.words[i])
            return false;
    return true;
}

bool bigint::operator!=(const bigint &a)
{
    return !(*this == a);
}

bool bigint::operator>(const bigint &a)
{
    for (int i = 0; i < WORDCNT; ++i)
    {
        if (this->words[i] > a.words[i])
            return true;
        if (this->words[i] < a.words[i])
            return false;
        // if equal, keep going
    }
    return false; // all words equal
}

bool bigint::operator<(const bigint &a)
{
    for (int i = 0; i < WORDCNT; ++i)
    {
        if (this->words[i] < a.words[i])
            return true;
        if (this->words[i] > a.words[i])
            return false;
    }
    return false; // all words equal
}

bool bigint::operator>=(const bigint &a)
{
    return !(*this < a);
}

bool bigint::operator<=(const bigint &a)
{
    return !(*this > a);
}

bool bigint::get_bit(int bit) const
{
    if (bit < 0 || bit >= 512)
        throw std::invalid_argument("index out of range!");

    int word_move = bit / BIT_CNT_WRD;
    int bit_move = bit % BIT_CNT_WRD;
    int word_index = WORDCNT - 1 - word_move;
    uint64_t and_operand = 1;
    and_operand <<= bit_move;
    return this->words[word_index] & and_operand;
}

void bigint::set_bit(int bit, bool value)
{
    if (bit < 0 || bit >= 512)
        throw std::invalid_argument("index out of range!");

    int word_move = bit / BIT_CNT_WRD;
    int bit_move = bit % BIT_CNT_WRD;
    int word_index = WORDCNT - 1 - word_move;
    uint64_t and_operand = 1;
    and_operand <<= bit_move;
    if (value)
        this->words[word_index] |= and_operand;
    else
    {
        and_operand = ~and_operand;
        this->words[word_index] &= and_operand;
    }
}

bigint::bigint(uint64_t cst)
{
    for (int i = 0; i < WORDCNT; i++)
        this->words[i] = 0;
    this->words[WORDCNT - 1] = cst;
}

bool bigint::is_even()
{
    return !(this->words[WORDCNT - 1] & RM_BIT_64);
}

bigint &bigint::operator+=(const bigint &a)
{
    *this = *this + a;
    return *this;
}

bigint &bigint::operator-=(const bigint &a)
{
    *this = *this - a;
    return *this;
}

bigint &bigint::operator*=(const bigint &a)
{
    *this = *this * a;
    return *this;
}

bigint &bigint::operator/=(const bigint &a)
{
    *this = *this / a;
    return *this;
}

bigint &bigint::operator%=(const bigint &a)
{
    *this = *this % a;
    return *this;
}

std::string bigint::debugstring(int n)
{
    std::string result = "|";
    for (int i = WORDCNT - n; i < WORDCNT; i++)
        result += __ui64_to_hex_str(this->words[i]) + "|";
    return result;
}

//////////////////////////////////////////
/// stream things. they are reverse //////
//////////////////////////////////////////

std::ostream &operator<<(std::ostream &os, const bigint &num)
{
    std::string result{};
    bool pad = false;
    for (int i = 0; i < WORDCNT; i++)
    {
        if (num.words[i] < 1 && !pad)
            continue;
        else if (num.words[i] >= 1 && !pad)
        {
            result += __ui64_to_hex_unpadded(num.words[i]);
            pad = true;
        }
        else
            result += __ui64_to_hex_str(num.words[i]);
    }
    std::reverse(result.begin(), result.end());
    os << result;
    return os;
}

std::istream &operator>>(std::istream &is, bigint &num)
{
    std::string input;
    is >> input;
    bigint temp(input);
    num = temp;
    return is;
}

//////////////////////////////////////////
///// BEZOUT ONLY METHODS ////////////////
//////////////////////////////////////////

bool bigint::__bz_get_negflag()
{
    return this->__bz_neg;
}

void bigint::__bz_set_negflag(bool sgn)
{
    this->__bz_neg = sgn;
}

//////////////////////////////////////////
/// other functions related to bigint below
//////////////////////////////////////////

bigint addMod(bigint a, bigint b, bigint n) // computes (a + b) % n
{
    return (a + b < n) ? a + b : a + b - n;
}

bigint mulMod(bigint a, bigint b, bigint n)
{
    bigint result; // starts at 0;
    bigint mul_addant = a;
    for (int i = 0; i < WORDCNT * BIT_CNT_WRD; i++)
    {
        if (b.get_bit(i))
            result = addMod(result, mul_addant, n);
        mul_addant = addMod(mul_addant, mul_addant, n); // this instead of a shift left.
    }
    return result;
}

bigint powMod(bigint b, bigint x, bigint n)
{
    bigint answer;
    answer.words[WORDCNT - 1] = 1;
    for (int i = WORDCNT * BIT_CNT_WRD - 1; i >= 0; i--)
    {
        answer = mulMod(answer, answer, n);
        if (x.get_bit(i))
            answer = mulMod(answer, b, n);
    }
    return answer;
}

bigint gcd(bigint a, bigint b)
{
    bigint answer;
    answer.words[WORDCNT - 1] = 1;
    if (a == bigint{})
        return b;
    if (b == bigint{})
        return a;
    while (a.is_even() && b.is_even())
    {
        a = a >> 1;
        b = b >> 1;
        answer = answer << 1;
    }

    while (a > bigint{} && b > bigint{})
    {
        while (a.is_even() && a != bigint{})
            a = a >> 1;
        while (b.is_even() && b != bigint{})
            b = b >> 1;
        bigint diff = (a > b) ? a - b : b - a;
        if (a > b)
            a = diff;
        else
            b = diff;
    }
    answer = (a > bigint{}) ? answer * a : answer * b;
    return answer;
}

bigint random_bigint(bigint a, bigint b)
{
    if (b <= a)
        throw std::invalid_argument("upper bound cannot be less than lower bound!!!");
    bigint thing;
    static std::mt19937_64 gen(std::random_device{}()); // seed once
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
    for (int i = 0; i < WORDCNT; i++)
        thing.words[i] = dist(gen);

    return (thing % (b - a)) + a;
}

bool rabin_miller(bigint n, int iterations)
{
    // bigint one, two; // an embarrasment
    // one.words[WORDCNT - 1] = 1;
    // two.words[WORDCNT - 1] = 2;

    if (n < bigint(2)) // n < two
        return false;
    if (n == bigint(2) || n == bigint(3)) // n == two || n == (one + two)
        return true;                      // only prime to be even
    if (n.is_even())
        return false; // nothing other than 2 is an even prime

    // 1. decompose n: n - 1 = 2^s * d
    bigint d = n - bigint(1);    // one
    bigint two_to_s = bigint(1); // one
    int s = 0;
    while (d.is_even())
    {
        d = d >> 1;
        two_to_s << 1;
        s++;
    }

    for (int i = 0; i < iterations; i++)
    {
        bigint a = random_bigint(bigint(2), n - bigint(2));
        bigint x = powMod(a, d, n);
        bigint y;
        for (int i = 0; i < s; i++)
        {
            y = mulMod(x, x, n);                                          // instead of powMod(x, two, n);
            if (y == bigint(1) && x != bigint(1) && x != (n - bigint(1))) // y == one && x != one && x != (n - one)
                return false;
            x = y;
        }
        if (y != bigint(1))
            return false;
    }
    return true; // probably
}

// ex_gcd (bezout) specific operator functions here

bigint __bz_add(bigint a, bigint b)
{
    bigint result;
    if (!a.__bz_get_negflag() && !b.__bz_get_negflag()) // a and b POSITIVE;
        return a + b;
    else if (!a.__bz_get_negflag() && b.__bz_get_negflag()) // if b negative only
    {
        if (b > a) // in magnitude. here return neg
        {
            result = b - a;
            result.__bz_set_negflag(true);
            return result; // negative
        }
        else
            return (a - b);
    }
    else if (a.__bz_get_negflag() && !b.__bz_get_negflag()) // if a negative only
    {
        if (a > b) // in magnitude. here return neg
        {
            result = a - b;
            result.__bz_set_negflag(true);
            return result;
        }
        else
            return (b - a);
    }
    else // both negative
    {
        result = (a + b);
        result.__bz_set_negflag(true);
        return result;
    }
}

bigint __bz_sub(bigint a, bigint b) // a - b
{
    bigint result;
    if (!a.__bz_get_negflag() && !b.__bz_get_negflag()) // both positive
    {
        if (a < b)
        {
            result = b - a;
            result.__bz_set_negflag(true);
            return result;
        }
        else
            return a - b;
    }
    else if (!a.__bz_get_negflag() && b.__bz_get_negflag()) // b negative only
        return a + b;
    else if (a.__bz_get_negflag() && !b.__bz_get_negflag()) // a negative only
    {
        result = a + b;
        result.__bz_set_negflag(true);
        return result;
    }
    else // both negative, this now becomes b - a
    {
        if (b < a)
        {
            result = a - b;
            result.__bz_set_negflag(true);
            return result;
        }
        else
        {
            return b - a; // just the magnitude
        }
    }
}

bigint __bz_mul(bigint a, bigint b)
{
    bigint result;
    if (!a.__bz_get_negflag() && !b.__bz_get_negflag()) // both positive
    {
        return a * b;
    }
    else if (!a.__bz_get_negflag() && b.__bz_get_negflag()) // b negative only
    {
        result = a * b;
        result.__bz_set_negflag(true);
        return result;
    }
    else if (a.__bz_get_negflag() && !b.__bz_get_negflag()) // a negative only
    {
        result = a * b;
        result.__bz_set_negflag(true);
        return result;
    }
    else // both negative
    {
        return a * b;
    }
}

// ex_gcd (bezout) specific operator functions here
bigint bezout_ex_gcd(bigint a, bigint b, bigint &x, bigint &y, bool &neg_x, bool &neg_y)
{
    bigint x0(1), x1(0), xcur, y0(0), y1(1), ycur;
    while (b > bigint(0))
    {
        bigint tmp; // = a % b
        bigint q;   //  = a / bconsolidate this to just one operator (method) later. dont do it twice

        a.div_mod(b, q, tmp);

        a = b;
        b = tmp;
        if (b == bigint(0))
            break;
        xcur = __bz_sub(x0, __bz_mul(q, x1));
        ycur = __bz_sub(y0, __bz_mul(q, y1));
        x0 = x1;
        x1 = xcur;
        y0 = y1;
        y1 = ycur;
    }

    // normalizes bigints with their neg flag to their 2 comp form
    // if (xcur.__bz_get_negflag())
    // {
    //     xcur.__bz_set_negflag(false);
    //     xcur = xcur.get_twocomp_neg();
    // }

    // if (ycur.__bz_get_negflag())
    // {
    //     ycur.__bz_set_negflag(false);
    //     ycur = ycur.get_twocomp_neg();
    // }

    neg_x = xcur.__bz_get_negflag();
    neg_y = ycur.__bz_get_negflag();

    xcur.__bz_set_negflag(false);
    ycur.__bz_set_negflag(false);

    x = xcur;
    y = ycur;
    return a;
} // probably correct, needs testing

bool bezout_confirmation(bigint gcd, bigint a, bigint b, bigint x, bigint y, bool neg_x, bool neg_y)
{
    bigint sum;
    sum = (neg_x) ? sum - x * a : sum + x * a;
    sum = (neg_y) ? sum - y * b : sum + y * b;
    return gcd == sum;
}

void prettyprint_bigint(bigint a)
{
    if (a.__bz_get_negflag())
        std::cout << "NEG  ";
    else
        std::cout << "POS  ";
    std::cout << a.debugstring(4) << "\n";
}

///////////////////////////////////////////////////////
/////// bigint.cpp ////////////////////////////////////
///////////////////////////////////////////////////////

#include <fstream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "argument count inappropriate.\nExample usage: main <input file> <output file>\n";
        return -1;
    }
    std::ifstream inp(argv[1]);
    std::ofstream out(argv[2]);

    if (!inp)
    {
        std::cout << argv[1] << " cannot be accessed, aborting.\n";
        inp.close();
        out.close();
        return -1;
    }

    bigint p, q, e, d, _, gcd, phi;
    bool negd, neg_;
    inp >> p >> q >> e;
    phi = (p - bigint(1)) * (q - bigint(1));

    gcd = bezout_ex_gcd(e, phi, d, _, negd, neg_);
    if (gcd != bigint(1))
        out << "-1" << "\n";
    else
        out << d << "\n";
    std::cout << d.debugstring() << "\n";
    inp.close();
    out.close();
    return 0;
}