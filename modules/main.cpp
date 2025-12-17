#include "openssl_rsa_handler.h"
#include <chrono>

// testing main
// currently testing base64 parser

int main()
{
    std::string input("E:\\cryptography_prj_1\\cryptography_lab_1\\test_input.txt");
    openssl_rsa_handler(input);
    // auto start = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < 100; i++)
    // {
    //     prettyprint_bigint(random_bigint(bigint(0), bigint(0xFFFFFFFFFFFFFFFF)));
    // }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
}
