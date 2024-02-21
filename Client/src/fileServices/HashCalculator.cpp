#include "../../include/fileServices/HashCalculator.h"
#include <iostream>
#include <vector>
#include <openssl/evp.h>
#include <filesystem>
#include <sstream>

std::string HashCalculator::calculateHash(const std::string &input) {
    bool isFilePath = std::filesystem::exists(input);

    if (isFilePath) {
        std::cout << "Calculating hash for file " << input << std::endl;
        std::ifstream file(input, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + input);
        }
        return processFileInChunks(file);
    } else {
        std::cout << "Calculating hash for string " << input << std::endl;
        std::vector<char> content(input.begin(), input.end());
        return processChunk(content.data(), content.size());
    }
}

std::string HashCalculator::processFileInChunks(std::ifstream &file) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    std::vector<char> buffer(4096);
    while (file.read(buffer.data(), buffer.size())) {
        EVP_DigestUpdate(ctx, buffer.data(), file.gcount());
    }
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    EVP_DigestFinal_ex(ctx, hash, &lengthOfHash);
    EVP_MD_CTX_free(ctx);

    return finalizeHash(hash, lengthOfHash);
}

std::string HashCalculator::processChunk(const char* data, size_t size) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, data, size);
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    EVP_DigestFinal_ex(ctx, hash, &lengthOfHash);
    EVP_MD_CTX_free(ctx);

    return finalizeHash(hash, lengthOfHash);
}

std::string HashCalculator::finalizeHash(const unsigned char* digest, unsigned int length) {
    std::stringstream ss;
    for (unsigned int i = 0; i < length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return ss.str();
}
