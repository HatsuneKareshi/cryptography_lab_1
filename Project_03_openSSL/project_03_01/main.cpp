#include "openssl_rsa_handler.h"
#include <chrono>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "wrong arguments. syntax: .exe <key-file-name>";
        return -1;
    }

    std::ifstream openssl(argv[1]);
    if (openssl.is_open() != true)
    {
        std::cout << "key file inaccessible\n";
        return -1;
    }

    std::string header{};
    std::string base64_payload = "";

    std::getline(openssl, header, '\n'); // read header

    while (!openssl.eof()) // get the rest of the payload
    {
        std::string tmp{};
        std::getline(openssl, tmp, '\n');
        if (tmp != PUBLKEY_ENDER && tmp != PRIVKEY_ENDER)
            base64_payload += tmp;
        else
            break;
    }
    if (header == PUBLKEY_HEADER)
    {
        std::cout << "public key detected\nrelevant information:\n";
        publicKey plk = __getpublicKey(base64_payload);
        std::cout << "N: " << plk.N.debugstring() << "\n";
        std::cout << "e: " << plk.e.debugstring() << "\n";
    }

    else if (header == PRIVKEY_HEADER)
    {
        std::cout << "private key detected\nrelevant information:\n";
        privatKey pvk = __getprivatKey(base64_payload);
        std::cout << "N: " << pvk.N.debugstring() << "\n";
        std::cout << "e: " << pvk.e.debugstring() << "\n";
        std::cout << "d: " << pvk.d.debugstring() << "\n";
        std::cout << "p: " << pvk.p.debugstring() << "\n";
        std::cout << "q: " << pvk.q.debugstring() << "\n";
        std::cout << "p's primality: " << (rabin_miller(pvk.p, 5) ? "true" : "false") << "\n";
        std::cout << "q's primality: " << (rabin_miller(pvk.q, 5) ? "true" : "false") << "\n";
        std::cout << "verifying N == p * q: " << (pvk.N == pvk.p * pvk.q ? "true" : "false") << "\n";
    }
    openssl.close();
}
