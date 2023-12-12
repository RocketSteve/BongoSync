#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <string>

class HashCalculator {
public:
    static std::string calculateHash(const std::string& filePath);
};

#endif // HASHCALCULATOR_H
