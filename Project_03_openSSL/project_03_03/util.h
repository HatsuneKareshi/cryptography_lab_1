#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "bigint.h"

long file_length(std::string filename);
std::vector<uint8_t> get_byte_stream_from_file(std::string filename);
void write_byte_stream_to_file(std::vector<uint8_t> payload, std::string filename);
bigint byte_stream_to_bigint(std::vector<uint8_t> pl);
std::vector<uint8_t> bigint_to_byte_stream(bigint bi, int byte_to_extract);
