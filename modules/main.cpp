// #include "openssl_rsa_handler.h"
// #include <chrono>
#include "bigint.h"

// testing main
// currently testing base64 parser

int main()
{
    // std::string input("E:\\cryptography_prj_1\\cryptography_lab_1\\test_input.txt");
    // openssl_rsa_handler(input);
    // test & and | later
    std::string sa, sb;
    std::cin >> sa >> sb;
    std::reverse(sa.begin(), sa.end());
    std::reverse(sb.begin(), sb.end());
    bigint a(sa), b(sb);
    std::cout << a.debugstring() << "\n";
    std::cout << b.debugstring() << "\n";
    std::cout << (a & b).debugstring() << "\n";
    std::cout << "paste test result here verbatim\n";
    std::string stest;
    std::cin >> stest;
    std::reverse(stest.begin(), stest.end());
    bigint test(stest);
    std::cout << "is correct? " << ((a & b) == test);
}

// auto start = std::chrono::high_resolution_clock::now();
// for (int i = 0; i < 100; i++)
// {
//     prettyprint_bigint(random_bigint(bigint(0), bigint(0xFFFFFFFFFFFFFFFF)));
// }
// auto end = std::chrono::high_resolution_clock::now();
// auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

// std::cout << "Time taken: " << duration.count() << " ms" << std::endl;