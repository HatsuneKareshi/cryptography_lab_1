#include "base64_parser.h"
#include <chrono>

// testing main
// currently testing base64 parser

int main()
{
    std::string a{};
    std::cin >> a;
    tester(base64_to_bytes(a));
}
