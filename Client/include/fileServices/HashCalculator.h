#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <string>
#include "blake3.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>


class HashCalculator {
public:
    static std::string calculateHash(const std::string &filePath);

private:
    static std::string processFileInChunks(std::ifstream &file);
    static std::string processChunk(const char* data, size_t size);
    static std::string finalizeHash(blake3_hasher &hasher);
};

#endif // HASHCALCULATOR_H
