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
    static std::string calculateHash(const std::string& filePath);
};

#endif // HASHCALCULATOR_H
