#include "../../include/fileServices/HashCalculator.h"
#include <iostream>

std::string HashCalculator::calculateHash(const std::string &input) {
    std::vector<char> content;
    bool isFilePath = std::filesystem::exists(input);

    if (isFilePath) {
        std::cout << "Calculating hash for file " << input << std::endl;
        std::ifstream file(input, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + input);
        }

        // Process the file in chunks
        return processFileInChunks(file);
    } else {
        std::cout << "Calculating hash for string " << input << std::endl;
        content = std::vector<char>(input.begin(), input.end());

        // Process the entire string as a single chunk
        return processChunk(content.data(), content.size());
    }
}

std::string HashCalculator::processFileInChunks(std::ifstream &file) {
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    std::vector<char> buffer(4096);

    while (file.read(buffer.data(), buffer.size())) {
        blake3_hasher_update(&hasher, buffer.data(), file.gcount());
    }
    // Handle the last chunk if there is any
    if (file.gcount() > 0) {
        blake3_hasher_update(&hasher, buffer.data(), file.gcount());
    }

    return finalizeHash(hasher);
}

std::string HashCalculator::processChunk(const char* data, size_t size) {
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, data, size);

    return finalizeHash(hasher);
}

std::string HashCalculator::finalizeHash(blake3_hasher &hasher) {
    uint8_t output[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);

    std::stringstream ss;
    for (int i = 0; i < BLAKE3_OUT_LEN; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(output[i]);
    }

    return ss.str();
}