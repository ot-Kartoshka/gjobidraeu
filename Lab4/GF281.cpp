#include "GF281.h"
#include <cstring>
#include <cmath>

const int P = 2 * M + 1;

GF281::GF281() {
    std::memset(data, 0, sizeof(data));
}

GF281::GF281(const std::string& bitString) {
    std::memset(data, 0, sizeof(data));
    int len = bitString.length();

    for (int i = 0; i < len && i < M; i++) {
        if (bitString[i] == '1') {
            setBit(i, 1);
        }
    }
}

int GF281::getBit(int pos) const {
    if (pos < 0 || pos >= M) return 0;
    return (data[pos / 64] >> (pos % 64)) & 1ULL;
}

void GF281::setBit(int pos, int val) {
    if (pos < 0 || pos >= M) return;
    if (val) data[pos / 64] |= (1ULL << (pos % 64));
    else     data[pos / 64] &= ~(1ULL << (pos % 64));
}

GF281 GF281::operator+(const GF281& other) const {
    GF281 res;
    for (int i = 0; i < WORDS; i++) {
        res.data[i] = this->data[i] ^ other.data[i];
    }
    return res;
}

GF281 GF281::square() const {
    return rotateRight(1);
}

GF281 GF281::rotateRight(int shift) const {
    GF281 res;
    for (int i = 0; i < M; i++) {
        int oldIdx = (i - shift) % M;
        if (oldIdx < 0) oldIdx += M;
        res.setBit(i, getBit(oldIdx));
    }
    return res;
}

GF281 GF281::rotateLeft(int shift) const {
    GF281 res;

    for (int i = 0; i < M; i++) {
        int oldIdx = (i + shift) % M;
        res.setBit(i, getBit(oldIdx));
    }
    return res;
}

std::vector<std::vector<int>> GF281::computeLambdaMatrix() {
    std::vector<std::vector<int>> lambda(M, std::vector<int>(M, 0));

    std::vector<int> pow2(M);
    int val = 1;
    for (int i = 0; i < M; i++) {
        pow2[i] = val;
        val = (val * 2) % P;
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            int p2i = pow2[i];
            int p2j = pow2[j];

            bool cond1 = (p2i + p2j) % P == 1;
            bool cond2 = (p2i - p2j + P) % P == 1;
            bool cond3 = (-p2i + p2j + P) % P == 1;
            bool cond4 = (-p2i - p2j + 2 * P) % P == 1;

            if (cond1 || cond2 || cond3 || cond4) {
                lambda[i][j] = 1;
            }
            else {
                lambda[i][j] = 0;
            }
        }
    }
    return lambda;
}

GF281 GF281::multiply(const GF281& other, const std::vector<std::vector<int>>& lambda) const {
    GF281 z;

    for (int k = 0; k < M; k++) {
        GF281 u_shifted = this->rotateLeft(k);
        GF281 v_shifted = other.rotateLeft(k);

        int sum = 0;

        std::vector<int> tempRow(M, 0);
        for (int c = 0; c < M; c++) {
            int s = 0;
            for (int r = 0; r < M; r++) {
                if (u_shifted.getBit(r) && lambda[r][c]) {
                    s++;
                }
            }
            tempRow[c] = s % 2; // mod 2
        }

        for (int c = 0; c < M; c++) {
            if (tempRow[c] && v_shifted.getBit(c)) {
                sum++;
            }
        }

        if (sum % 2 != 0) {
            z.setBit(k, 1);
        }
    }
    return z;
}

int GF281::trace() const {
    int t = 0;
    for (int i = 0; i < M; i++) {
        if (getBit(i)) t++;
    }
    return t % 2;
}

GF281 GF281::one() {
    GF281 o;
    for (int i = 0; i < M; i++) o.setBit(i, 1);
    return o;
}

GF281 GF281::zero() {
    return GF281();
}

std::string GF281::to_string() const {
    std::string s = "";
    for (int i = 0; i < M; i++) {
        s += getBit(i) ? "1" : "0";
    }
    return s;
}

GF281 GF281::inverse(const std::vector<std::vector<int>>& lambda) const {

    std::string expBin = "";
    for (int i = 0; i < M - 1; i++) expBin += "1";
    expBin += "0";

    return power(expBin, lambda);
}


GF281 GF281::power(const std::string& exponentBin, const std::vector<std::vector<int>>& lambda) const {
    GF281 res = one();
    GF281 base = *this;

    for (char c : exponentBin) {
        res = res.multiply(res, lambda);

        res = res.square();

        if (c == '1') {
            res = res.multiply(base, lambda);
        }
    }
    return res;
}