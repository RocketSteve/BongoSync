#include "../include/HashCalculator.h"


std::string HashCalculator::calculateHash(const std::string& input) {
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    // If the input is a file path
    if (std::ifstream file{input, std::ios::binary}; file) {
        std::vector<char> buffer(1024 * 1024);  // 1MB buffer
        while (file.read(buffer.data(), buffer.size()) || file.gcount()) {
            blake3_hasher_update(&hasher, buffer.data(), file.gcount());
        }
    } else {
        // If the input is a regular string
        blake3_hasher_update(&hasher, input.data(), input.size());
    }

    unsigned char hash[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, hash, BLAKE3_OUT_LEN);

    std::stringstream ss;
    for (unsigned char c : hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return ss.str();
}