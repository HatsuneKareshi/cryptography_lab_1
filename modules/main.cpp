#include "openssl_rsa_handler.h"
#include "util.h"
#include "base64_parser.h"
// testing main
// currently testing utils

int main(int argc, char **argv)
{
    std::cout << "HELLO\n";
    std::string header, payload;
    std::ifstream keyfile(argv[1]);
    std::getline(keyfile, header, '\n');
    while (!keyfile.eof())
    {
        std::string temp;
        std::getline(keyfile, temp, '\n');
        if (temp != PRIVKEY_ENDER && temp != PUBLKEY_ENDER)
            payload += temp;
        else
            break;
    }

    privatKey prvkey = __getprivatKey(payload); // get the private key here
    std::string cipherstring;
    std::cin >> cipherstring;
    std::reverse(cipherstring.begin(), cipherstring.end());
    bigint c(cipherstring);

    std::cout << "c: " << c.debugstring() << "\n";
    std::cout << "d: " << prvkey.d.debugstring() << "\n";
    std::cout << "N: " << prvkey.N.debugstring() << "\n";
    bigint m = powMod(c, prvkey.d, prvkey.N);
    std::cout << "m: " << m.debugstring() << "\n";
}
// std::string input;
// std::cin >> input;
// std::reverse(input.begin(), input.end());
// bigint tes(input);

// std::cout << "=========\n";
// std::cout << tes.debugstring() << "\n\n";
// tester(bigint_to_byte_stream(tes, 96));
// bigint tes2 = byte_stream_to_bigint(bigint_to_byte_stream(tes, 96));
// std::cout
//     << "\n\n"
//     << tes2.debugstring();
// std::cout << "equal? " << (tes == tes2) << "\n";