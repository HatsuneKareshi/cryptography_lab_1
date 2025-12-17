#include "base64_parser.h"

const int BITLEN_B64 = 6;
const int CONVEN_BLEN = 8;
// CHAR_MAPPING

std::vector<uint8_t>
base64_to_bytes(std::string b64)
{
    long length = b64.size();
    if (length % 4 != 0)
        throw std::invalid_argument("base64 content corrupted or incomplete!!!\n");
    std::vector<uint8_t> ret_buffer = {};
    std::stack<uint8_t> reverser;
    uint32_t quad_buffer = 0x0;
    int paddmark = 0;
    for (long i = 0, buf_cnt = 0; i < length; i++)
    {
        if (b64[i] != '=')
        {
            quad_buffer = quad_buffer << BITLEN_B64;
            quad_buffer = quad_buffer | CHAR_MAPPING.at(b64[i]); // remember this acts as a stack. use another stack later to reverse its reversal
            buf_cnt++;                                           // counts the number how many times this has happened
        }
        else
        {
            quad_buffer = quad_buffer << BITLEN_B64;
            quad_buffer = quad_buffer | 0x0; // this is just padding
            buf_cnt++;
            paddmark++;
        }

        if (buf_cnt >= 4)
        {
            buf_cnt = 0;
            for (int i = 0; i < 3; i++) // in batches of 3 here.
            {
                uint8_t byte = 0x0;
                byte = quad_buffer & 0xFF; // this would be 0x1111 1111, we are grabbing 6 bits off of the lsb end
                quad_buffer = quad_buffer >> CONVEN_BLEN;
                reverser.push(byte); // later when we pop it out it should be in the correct order, ready to be shoved back into the
            }
            for (int i = 0; i < 3; i++)
            {
                ret_buffer.push_back(reverser.top());
                reverser.pop();
            }
        }
    }
    for (int i = 0; i < paddmark; i++)
        ret_buffer.pop_back();
    return ret_buffer;
}

void tester(std::vector<uint8_t> payload)
{
    for (auto byte : payload)
    {
        printf("%02X ", byte);
    }
    std::cout << "\n";
}