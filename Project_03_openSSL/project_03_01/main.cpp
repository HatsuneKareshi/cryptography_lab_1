#include "openssl_rsa_handler.h"
#include "base64_parser.h"
#include <cstring>

// experimental implementation for task 2. expect more modifications to come later

int main(int argc, char **argv) // main <keyfile> -enc/dec <source> <destination>
{
    if (argc != 2)
    {
        std::cout << "incorrect arguments. syntax: .exe <key-file-name>\n";
        return -1;
    }
    std::cout << "Note: all integers printed left to right, most significant to least significant.\neach block of hex within bars || represent 64 bits of data.\n";
    bool file_reported = false;
    try
    {

        publicKey plk = parsePubkeyFile(argv[1]);
        std::cout << "=====PUBLIC KEY=====\n";
        std::cout << "modulus length: " << plk.NBytecount << " bytes\n";
        std::cout << "N= " << plk.N.debugstring() << "\n";
        std::cout << "e= " << plk.e.debugstring() << "\n";
        std::cout << "=====PUBLIC KEY=====\n";
        file_reported = true;
    }
    catch (std::exception e)
    {
    }
    try // very janky yes
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
        file_reported = true;
    }
    catch (std::exception e)
    {
    }

    if (!file_reported)
        std::cout << "file inaccessble or not a key file!\n";

    return 0;
}
