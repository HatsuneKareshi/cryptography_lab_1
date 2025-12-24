// #include "openssl_rsa_handler.h"
// #include <chrono>
// #include "bigint.h"
#include "util.h"
#include "base64_parser.h"
// testing main
// currently testing utils

int main()
{
    std::string input;
    std::cin >> input;
    std::reverse(input.begin(), input.end());
    bigint tes(input);

    std::cout << "=========\n";
    std::cout << tes.debugstring() << "\n\n";
    tester(bigint_to_byte_stream(tes, 96));
    bigint tes2 = byte_stream_to_bigint(bigint_to_byte_stream(tes, 96));
    std::cout
        << "\n\n"
        << tes2.debugstring();
    std::cout << "equal? " << (tes == tes2) << "\n";
}
