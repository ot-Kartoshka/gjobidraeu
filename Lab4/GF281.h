#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

const int M = 281;
const int WORDS = 5;

class GF281 {
private:
    uint64_t data[WORDS];

public:
    GF281();
    GF281(const std::string& bitString);

    int getBit(int pos) const;
    void setBit(int pos, int val);

    GF281 operator+(const GF281& other) const;

    GF281 square() const;

    GF281 multiply(const GF281& other, const std::vector<std::vector<int>>& lambda) const;

    int trace() const;

    GF281 inverse(const std::vector<std::vector<int>>& lambda) const;

    GF281 power(const std::string& exponentBin, const std::vector<std::vector<int>>& lambda) const;

    GF281 rotateRight(int shift) const; 
    GF281 rotateLeft(int shift) const;  

    std::string to_string() const;
    static GF281 zero();
    static GF281 one();

    static std::vector<std::vector<int>> computeLambdaMatrix();
};