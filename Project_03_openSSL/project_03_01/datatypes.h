#include "bigint.h"

struct publicKey
{
    bigint N, e;
};

struct privatKey
{
    bigint N, p, q, d, e;
};
