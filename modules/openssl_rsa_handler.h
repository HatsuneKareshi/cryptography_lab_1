#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "base64_parser.h"
#include "bigint.h"

// this finds the length right after 0x30, a sequence marker.
// usually it will look like 0x30 0xxx. if 0xxx < 0x80, that byte itself is the length
// if not, then whatever bit is left (excluding the most significant bit)
// represents the amount of bytes after that which represents the length of what to read next
// note that this ADVANCES the cursor to the first actual data after seq length
long __get_length(const std::vector<uint8_t> &payload, long &cursor, uint8_t bytecode);
void openssl_rsa_handler(std::string filename);

const std::string PRIVKEY_HEADER = "-----BEGIN PRIVATE KEY-----";
const std::string PRIVKEY_ENDER = "-----END PRIVATE KEY-----";
const std::string PUBLKEY_HEADER = "-----BEGIN PUBLIC KEY-----";
const std::string PUBLKEY_ENDER = "-----END PUBLIC KEY-----";