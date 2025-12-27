#include "openssl_rsa_handler.h"
#include "util.h"
#include "base64_parser.h"
#include <cstring>

// experimental implementation for task 2. expect more modifications to come later

int main(int argc, char **argv) // main <keyfile> -enc/dec <source> <destination>
{
    if (argc != 5)
    {
        std::cout << "wrong argument placement. usage:\n$ .exe <keyfile> -enc <source> <destination>\n                 -dec\n"; // 17
        return -1;
    }
    if (strcmp(argv[2], "-enc") && strcmp(argv[2], "-dec"))
    {
        std::cout << "wrong argument. action argument is position 2 (starting from 0) and is either \"-enc\" or \"-dec\"\n";
        return -1;
    }
    if (!strcmp(argv[2], "-enc")) // encryption
    {
        std::string srcfn(argv[3]), dstfn(argv[4]), keyfn(argv[1]);
        publicKey plk;

        try
        {
            plk = parsePubkeyFile(keyfn);
        }
        catch (std::exception e)
        {
            std::cout << "wrong key type!!\n";
            return -1;
        }

        std::vector<uint8_t> cypherBytestream{};
        cypherBytestream.push_back(0x00);
        cypherBytestream.push_back(0x02);
        int padLength = 64 - 2 - 1 - file_length(srcfn);
        if (padLength < 0)
        {
            std::cout << "file too long for 64byte RSA!\n";
            return -1;
        }
        for (int i = 0; i < padLength; i++)
            cypherBytestream.push_back(0xBF); // soo many Boyfriends AAAAAAAAAAAAAAAAAAAAAAAAAAAA (or none if theres 61 bytes of data);
        cypherBytestream.push_back(0x00);
        for (auto &b : get_byte_stream_from_file(srcfn))
            cypherBytestream.push_back(b);

        std::cout << "cypher bytedump\n";
        tester(cypherBytestream);

        bigint m = byte_stream_to_bigint(cypherBytestream);
        bigint c = powMod(m, plk.e, plk.N);

        std::cout << cypherBytestream.size() << "\n";

        write_byte_stream_to_file(bigint_to_byte_stream(c, 64), dstfn);
    }
    if (!strcmp(argv[2], "-dec"))
    {
        std::string srcfn(argv[3]), dstfn(argv[4]), keyfn(argv[1]);
        privatKey pvk;

        try
        {
            pvk = parsePrvkeyFile(keyfn);
        }
        catch (std::exception e)
        {
            std::cout << "wrong key type!!\n";
            return -1;
        }

        bigint c = byte_stream_to_bigint(get_byte_stream_from_file(srcfn));
        bigint m = powMod(c, pvk.d, pvk.N);

        std::vector<uint8_t> mssgBytestream = bigint_to_byte_stream(m, 64);
        std::vector<uint8_t> filebytestream;
        int delinatorCNT = 0;
        for (auto &mb : mssgBytestream)
        {
            if (delinatorCNT >= 2)
                filebytestream.push_back(mb);
            if (delinatorCNT < 2 && mb == 0x00)
                delinatorCNT++;
        }

        write_byte_stream_to_file(filebytestream, dstfn);
    }
    return 0;
}
