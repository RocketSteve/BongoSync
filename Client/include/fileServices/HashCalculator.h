#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <string>
#include <fstream>

class HashCalculator {
public:
    static std::string calculateHash(const std::string &input);
private:
    static std::string processFileInChunks(std::ifstream &file);
    static std::string processChunk(const char* data, size_t size);
    static std::string finalizeHash(const unsigned char* digest, unsigned int length);
};

#endif // HASHCALCULATOR_H
