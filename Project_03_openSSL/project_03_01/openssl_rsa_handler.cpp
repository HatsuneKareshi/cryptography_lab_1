#include "openssl_rsa_handler.h"

long __get_length(const std::vector<uint8_t> &payload, long &cursor, uint8_t bytecode = 0x30) // defaults to SEQUENCE
{
    if (payload[cursor] != bytecode)
        throw std::invalid_argument("incorrect bytecode found");
    cursor++; // advances to read the first bit length
    if (payload[cursor] < 0x80)
        return long(payload[cursor++]); // less than 0x80, that byte itself is the length. also advance it into reading pos
    else
    {
        int length_len = payload[cursor] & 0x7F; // first like 7 bits;
        long len_buf = 0x0;
        cursor++; // advance to read the actual length
        for (int i = 0; i < length_len; i++)
        {
            len_buf <<= 8;
            len_buf |= payload[cursor];
            cursor++;
        } // already advanced into reading position.
        return len_buf;
    }
}

publicKey __getpublicKey(std::string base64_payload)
{
    publicKey plk;
    std::vector<uint8_t> payload = base64_to_bytes(base64_payload);
    bigint N, e;
    // tester(payload);
    long cursor = 0;
    long length = payload.size();
    __get_length(payload, cursor);
    // this is the entire length. do not add it itn
    long len_2 = __get_length(payload, cursor);
    cursor += len_2;                                   // this skips over the algoID
    __get_length(payload, cursor, 0x03);               // skips the inner sequence start
    cursor++;                                          // skip the padder 0x0
    __get_length(payload, cursor);                     // step into the inner sequence
    int NLength = __get_length(payload, cursor, 0x02); // upcoming is a 0x02 tag for the integer
                                                       // length of the N
    for (int i = 0; i < NLength; i++)
    {
        uint8_t temp = 0x0;
        temp = payload[cursor];
        cursor++;
        N = N << 8;
        N += bigint(temp); // build the N
    }
    int eLength = __get_length(payload, cursor, 0x02); // length of e;
    for (int i = 0; i < eLength; i++)
    {
        uint8_t temp = 0x0;
        temp = payload[cursor];
        cursor++;
        e = e << 8;
        e += bigint(temp); // build the N
    }
    // std::cout << "final cursor position: " << cursor << "\n";
    // prettyprint_bigint(N);
    // prettyprint_bigint(e);
    plk.N = N;
    plk.e = e;
    return plk;
}

privatKey __getprivatKey(std::string base64_payload)
{
    privatKey pvk;
    std::vector<uint8_t> payload = base64_to_bytes(base64_payload);
    bigint N, e, d, p, q;
    // tester(payload);
    long length = payload.size();
    long cursor = 0;
    __get_length(payload, cursor); // sequence;
    __get_length(payload, cursor, 0x02);
    std::cout << "version: " << int(payload[cursor]) << "\n";
    cursor++;
    long algoID_len = __get_length(payload, cursor);
    cursor += algoID_len;
    __get_length(payload, cursor, 0x04); // octet string whatever idk man
    __get_length(payload, cursor);
    __get_length(payload, cursor, 0x02); // version, skip
    cursor++;                            // skip
    long NLength = __get_length(payload, cursor, 0x02);
    for (long i = 0; i < NLength; i++)
    {
        uint8_t temp = 0x0;
        temp = payload[cursor];
        cursor++;
        N = N << 8;
        N += bigint(temp); // build the N
    }
    long eLength = __get_length(payload, cursor, 0x02);
    for (long i = 0; i < eLength; i++)
    {
        uint8_t temp = 0x0;
        temp = payload[cursor];
        cursor++;
        e = e << 8;
        e += bigint(temp); // build the N
    }
    long dLength = __get_length(payload, cursor, 0x02);
    for (long i = 0; i < dLength; i++)
    {
        uint8_t temp = 0x0;
        temp = payload[cursor];
        cursor++;
        d = d << 8;
        d += bigint(temp); // build the N
    }
    long pLength = __get_length(payload, cursor, 0x02);
    for (long i = 0; i < pLength; i++)
    {
        uint8_t temp = 0x0;
        temp = payload[cursor];
        cursor++;
        p = p << 8;
        p += bigint(temp); // build the N
    }
    long qLength = __get_length(payload, cursor, 0x02);
    for (long i = 0; i < qLength; i++)
    {
        uint8_t temp = 0x0;
        temp = payload[cursor];
        cursor++;
        q = q << 8;
        q += bigint(temp); // build the N
    }

    pvk.N = N;
    pvk.d = d;
    pvk.e = e;
    pvk.p = p;
    pvk.q = q;
    return pvk;
}
