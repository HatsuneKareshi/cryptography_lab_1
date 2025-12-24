#include "openssl_rsa_handler.h"
#include "util.h"
#include <iostream>
#include <cstring>
int main(int argc, char **argv) // intended use: main <key_file> --enc/dec <message> <cipher>
{
    if (argc != 5)
    {
        std::cout << "wrong arguments. proper use: main <key_file> --enc/dec <message> <cipher>"; // fixed 512 bit
        return -1;
    }

    std::ifstream keyfile(argv[1]);
    if (keyfile.is_open() == false)
        throw std::invalid_argument("key inaccesible!");

    std::string header, base64_payld;
    std::getline(keyfile, header, '\n'); // could go validate key here
    while (!keyfile.eof())
    {
        std::string tmp;
        std::getline(keyfile, tmp, '\n');
        if (tmp != PRIVKEY_ENDER && tmp != PUBLKEY_ENDER)
            base64_payld += tmp;
        else
            break;
    } // get key base64
    keyfile.close();

    for (int i = 0; i < argc; i++)
        std::cout << "\t" << argv[i] << "\n";
    if (!strcmp(argv[2], "--enc"))
    {
        // create the bytestream
        std::vector<uint8_t> cipher{};
        cipher.push_back(0x00); // makes sure m is less than n
        cipher.push_back(0x02); // specific 0x02 cipher identifier
        int filelength = file_length(argv[3]);
        int padlength = 64 - 2 - 1 - filelength;
        if (padlength < 0)
            throw std::invalid_argument("file exceeds length for 512 bit RSA ()");
        for (int i = 0; i < padlength; i++)
            cipher.push_back(0xFF); // pad
        cipher.push_back(0x00);
        for (auto &b : get_byte_stream_from_file(argv[3]))
            cipher.push_back(b);
        std::cout << "length " << cipher.size() << "\n";
        tester(cipher); // here cipher is the bytestream that becomes m

        // shove the cipher into bigint m
        bigint m; // message
        m = byte_stream_to_bigint(cipher);
        std::cout << "MESSAGE: " << m.debugstring() << "\n";

        // get keys
        publicKey pubkey = __getpublicKey(base64_payld);
        // actual encryption
        bigint c = powMod(m, pubkey.e, pubkey.N);
        std::cout << "CIPHER : " << c.debugstring() << "\n";
        // output straight to cypher file
        write_byte_stream_to_file(bigint_to_byte_stream(c, 64), std::string(argv[4]));
    }
    if (!strcmp(argv[2], "--dec"))
    {
        std::cout << "not implemented";
        return -1;
    }
}