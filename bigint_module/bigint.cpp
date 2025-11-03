#include "bigint.h"

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
}

bigint &bigint::operator=(const bigint &a)
{
    for (int i = 0; i < WORDCNT; i++)
        this->words[i] = a.words[i];
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

bool bigint::is_even()
{
    return !(this->words[WORDCNT - 1] & RM_BIT_64);
}

std::string bigint::debugstring()
{
    std::string result = "|";
    for (int i = 0; i < WORDCNT; i++)
        result += __ui64_to_hex_str(this->words[i]) + "|";
    return result;
}

/// other functions related to bigint below

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
    if (b < a)
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
    bigint one, two; // an embarrasment
    one.words[WORDCNT - 1] = 1;
    two.words[WORDCNT - 1] = 2;
    if (n == two)
        return true; // only prime to be even
    if (n.is_even())
        return false; // nothing other than 2 is an even prime

    // 1. decompose n: n - 1 = 2^s * d
    bigint d = n - one;
    bigint two_to_s = one;
    int s = 0;
    while (d.is_even())
    {
        d = d >> 1;
        two_to_s << 1;
        s++;
    }

    for (int i = 0; i < iterations; i++)
    {
        bigint a = random_bigint(one, n - one);
        bigint x = powMod(a, d, n);
        bigint y;
        for (int i = 0; i < s; i++)
        {
            y = mulMod(x, x, n); // instead of powMod(x, two, n);
            if (y == one && x != one && x != (n - one))
                return false;
            x = y;
        }
        if (y != one)
            return false;
    }
    return true; // probably
}