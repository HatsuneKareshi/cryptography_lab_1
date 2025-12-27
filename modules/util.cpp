#include "util.h"

long file_length(std::string filename)
{
    std::ifstream bin_in(filename, std::ios::binary);
    if (bin_in.is_open() != true)
        throw std::invalid_argument("file not reachable (file_length)");
    bin_in.seekg(0, std::ios::end);
    long rst = bin_in.tellg();
    bin_in.close();
    return rst;
}

std::vector<uint8_t> get_byte_stream_from_file(std::string filename)
{
    std::vector<uint8_t> payload{};
    std::ifstream bin_in(filename, std::ios::binary);
    if (bin_in.is_open() != true)
        throw std::invalid_argument("file not reachable (get_byte_stream_from_file)");
    char bt;
    while (bin_in.get(bt))
    {
        payload.push_back(uint8_t(bt));
    }
    // if (file_length(filename) != payload.size())
    //     throw std::invalid_argument("MISMATCH!!!");
    bin_in.close();
    return payload;
}

void write_byte_stream_to_file(std::vector<uint8_t> payload, std::string filename)
{
    std::ofstream bin_out(filename, std::ios::binary);
    for (auto &b : payload)
        bin_out.put(b);
    bin_out.close();
    return;
}

bigint byte_stream_to_bigint(std::vector<uint8_t> pl)
{
    if (WORDCNT * 64 < pl.size())
        throw std::invalid_argument("bytestream too long for bigint size of " + std::to_string(WORDCNT * 64) + " bits");
    bigint result;
    for (auto &b : pl)
    {
        result = result << 8;
        result += bigint(int(b));
    }
    return result;
}

std::vector<uint8_t> bigint_to_byte_stream(bigint bi, int byte_to_extract) // expect padded
{
    if (byte_to_extract * 8 > WORDCNT * 64)
        throw std::invalid_argument("byte to extract too many for bigint size of " + std::to_string(WORDCNT * 64) + " bits");
    std::vector<uint8_t> result{};
    bigint sacr(bi);
    std::cout << bi.debugstring() << "\n";
    std::cout << sacr.debugstring() << "\n";
    for (int i = 0; i < byte_to_extract; i++)
    {
        // printf("%02X ", sacr.getLSB8bits());
        result.push_back(sacr.getLSB8bits());
        sacr = sacr >> 8;
    }
    std::reverse(result.begin(), result.end());
    return result;
}