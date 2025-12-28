#pragma once
#include "bigint.h"

struct publicKey
{
    bigint N, e;
    int NBytecount;
};

struct privatKey
{
    bigint N, p, q, d, e;
    int NBytecount;
};
