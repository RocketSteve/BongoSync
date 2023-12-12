#include "../include/HashCalculator.h"
#include "blake3.h"  // Include the BLAKE3 header
#include <fstream>
#include <sstream>
#include <vector>

std::string HashCalculator::calculateHash(const std::string& filePath) {
    std::ifstream fileStream(filePath, std::ios::binary);
    if (!fileStream) {
        throw std::runtime_error("Could not open file for hashing: " + filePath);
    }

    // Initialize BLAKE3 hasher
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    // Read and hash the file content
    std::vector<char> buffer(1024 * 1024); // Buffer size can be adjusted
    while (fileStream.read(buffer.data(), buffer.size()) || fileStream.gcount()) {
        blake3_hasher_update(&hasher, buffer.data(), fileStream.gcount());
    }

    // Finalize the hash
    unsigned char hash[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, hash, BLAKE3_OUT_LEN);

    // Convert the hash to a hex string
    std::stringstream hashString;
    for (unsigned char c : hash) {
        hashString << std::hex << static_cast<int>(c);
    }

    return hashString.str();
}
