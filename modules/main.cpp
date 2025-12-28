#include "openssl_rsa_handler.h"
#include "util.h"
#include "base64_parser.h"
#include <cstring>
// bigint a(0xFFFFFFFFFFFFFFFF), b(0xFFFFFFFFFFFFFFFF);
// a = a << 64;
// b = b << 64;
// a = a | bigint(0xFFFFFFFFFFFFFFFF);
// b = b | bigint(0xFFFFFFFFFFFFFFFF);

// std::cout << a.debugstring() << "\n";
// std::cout << b.debugstring() << "\n";
// std::cout << (a + b).debugstring() << "\n";

int main(int argc, char **argv) // intended syntax: *.exe <keyfile> -sign/-vrfy <message> <signage>
{
    if (argc != 5)
    {
        std::cout << "wrong syntax, proper usage are: *.exe <keyfile> -sign <message> <signage>\n               -vrfy\n";
        return -1;
    }
    if (strcmp("-sign", argv[2]) && strcmp("-vrfy", argv[2]))
    {
        std::cout << "wrong argument. action argument is position 2 (starting from 0) and is either \"-sign\" or \"-vrfy\"\n";
        return -1;
    }

    if (!strcmp(argv[2], "-sign")) // uses the private key
    {
        std::string keyfn(argv[1]), mesfn(argv[3]), sgnfn(argv[4]);
        privatKey pvk;

        try
        {
            pvk = parsePrvkeyFile(keyfn);
        }
        catch (std::exception e)
        {
            std::cout << "error getting key, " << e.what() << "\n";
            return -1;
        }

        std::vector<uint8_t> signBytestream{};
        signBytestream.push_back(0x00);
        signBytestream.push_back(0x01); // must be 01 because this is signage not cypher
        int padLength = pvk.NBytecount - 2 - 1 - file_length(mesfn);
        if (padLength < 8)
        {
            std::cout << "file too long for key of this size!\n";
            return -1;
        }

        for (int i = 0; i < padLength; i++)
            signBytestream.push_back(0xFF); // padding. has to be 0xFF
        signBytestream.push_back(0x00);
        for (auto &b : get_byte_stream_from_file(mesfn))
            signBytestream.push_back(b);

        bigint m = byte_stream_to_bigint(signBytestream);
        bigint s = powMod(m, pvk.d, pvk.N); // s = m^d mod N.

        // std::cout << "m: " << m.debugstring() << "\n";
        // std::cout << "d: " << pvk.d.debugstring() << "\n";
        // std::cout << "N: " << pvk.N.debugstring() << "\n";
        // std::cout << "s: " << s.debugstring() << "\n";

        write_byte_stream_to_file(bigint_to_byte_stream(s, pvk.NBytecount), sgnfn);
    }
    if (!strcmp(argv[2], "-vrfy"))
    {
        std::string keyfn(argv[1]), mesfn(argv[3]), sgnfn(argv[4]);
        publicKey plk;
        try
        {
            plk = parsePubkeyFile(keyfn);
        }
        catch (std::exception e)
        {
            std::cout << "error getting key, " << e.what() << "\n";
            return -1;
        }
        if (plk.NBytecount > (WORDCNT * 8))
        {
            std::cout << "key too large for bigint max size\n";
            return -1;
        }

        bigint s = byte_stream_to_bigint(get_byte_stream_from_file(sgnfn));

        bigint m = powMod(s, plk.e, plk.N); // s^e mod N

        // std::cout << "s: " << s.debugstring() << "\n";
        // std::cout << "e: " << plk.e.debugstring() << "\n";
        // std::cout << "N: " << plk.N.debugstring() << "\n";
        // std::cout << "m: " << m.debugstring() << "\n";

        std::vector<uint8_t> paddedmssgBytestream = bigint_to_byte_stream(m, plk.NBytecount); // N length
        std::vector<uint8_t> unpaddedStream{};
        int deliniatorCNT = 0;
        for (auto &mb : paddedmssgBytestream)
        {
            if (deliniatorCNT >= 2)
                unpaddedStream.push_back(mb);
            if (deliniatorCNT < 2 && mb == 0x00)
                deliniatorCNT++;
        }

        // tester(paddedmssgBytestream);

        if (unpaddedStream == get_byte_stream_from_file(mesfn))
            std::cout
                << "message matches signature\n";
        else
            std::cout << "message does not match signature\n";
    }
    return 0;
}
