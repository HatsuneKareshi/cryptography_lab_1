#include "openssl_rsa_handler.h"
#include "base64_parser.h"
#include <cstring>

// experimental implementation for task 2. expect more modifications to come later

int main(int argc, char **argv) // main <keyfile> -pub/priv
{
    if (argc != 3)
    {
        std::cout << "incorrect arguments. syntax: \n.exe <key-file-name> -pub\n                     -priv\n";
        return -1;
    }
    std::cout << "Note: all integers printed left to right, most significant to least significant.\neach block of hex within bars || represent 64 bits of data.\n";
    if (strcmp(argv[2], "-pub") && strcmp(argv[2], "-priv"))
    {
        std::cout << "incorrect syntax. action specifier is in position 2, and is either -pub or -priv\n";
        return -1;
    }
    if (!strcmp(argv[2], "-pub"))
    {
        try
        {

            publicKey plk = parsePubkeyFile(argv[1]);
            std::cout << "=====PUBLIC KEY=====\n";
            std::cout << "modulus length: " << plk.NBytecount << " bytes\n";
            std::cout << "N= " << plk.N.debugstring() << "\n";
            std::cout << "e= " << plk.e.debugstring() << "\n";
            std::cout << "=====PUBLIC KEY=====\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "error parsing public key, " << e.what() << "\n";
            return -1;
        }
    }
    if (!strcmp(argv[2], "-priv"))
    {
        try
        {

            privatKey pvk = parsePrvkeyFile(argv[1]);
            std::cout << "=====PRIVATE KEY=====\n";
            std::cout << "modulus length: " << pvk.NBytecount << " bytes\n";
            std::cout << "N= " << pvk.N.debugstring() << "\n";
            std::cout << "e= " << pvk.e.debugstring() << "\n";
            std::cout << "d= " << pvk.d.debugstring() << "\n";
            std::cout << "p= " << pvk.p.debugstring() << "\n";
            std::cout << "q= " << pvk.q.debugstring() << "\n";
            std::cout << "=====PRIVATE KEY=====\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "error parsing public key, " << e.what() << "\n";
            return -1;
        }
    }
    return 0;
}
