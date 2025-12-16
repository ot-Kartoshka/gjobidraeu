#include "pch.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>

#include "../Lab4/GF281.cpp" 

std::string randomBitString() {
    std::string s = "";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    for (int i = 0; i < 281; i++) {
        s += std::to_string(dis(gen));
    }
    return s;
}

class GF281Test : public ::testing::Test {
protected:

    static const std::vector<std::vector<int>>& getLambda() {

        static std::vector<std::vector<int>> lambdaInstance = GF281::computeLambdaMatrix();
        return lambdaInstance;
    }
};


TEST_F(GF281Test, StringConstructorAndToString) {
    std::string input = "10101";
    GF281 a(input);
    EXPECT_EQ(a.getBit(0), 1);
    EXPECT_EQ(a.getBit(1), 0);
    EXPECT_EQ(a.getBit(2), 1);
}

TEST_F(GF281Test, ZeroAndOneConstants) {
    GF281 zero = GF281::zero();
    GF281 one = GF281::one();
    std::string sZero = zero.to_string();
    EXPECT_EQ(sZero.find('1'), std::string::npos);
}


TEST_F(GF281Test, AdditionNeutralElement) {
    GF281 a(randomBitString());
    GF281 zero = GF281::zero();
    GF281 res = a + zero;
    EXPECT_EQ(res.to_string(), a.to_string());
}

TEST_F(GF281Test, AdditionInverse) {
    GF281 a(randomBitString());
    GF281 res = a + a;
    EXPECT_EQ(res.to_string(), GF281::zero().to_string());
}

TEST_F(GF281Test, AdditionCommutativity) {
    GF281 a(randomBitString());
    GF281 b(randomBitString());
    EXPECT_EQ((a + b).to_string(), (b + a).to_string());
}


TEST_F(GF281Test, SquareIsCyclicShiftRight) {
    GF281 a;
    a.setBit(10, 1);
    GF281 sq = a.square();
    EXPECT_EQ(sq.getBit(11), 1); 
    EXPECT_EQ(sq.getBit(10), 0);
}

TEST_F(GF281Test, FreshmanDream) {
    GF281 a(randomBitString());
    GF281 b(randomBitString());
    GF281 lhs = (a + b).square();
    GF281 rhs = a.square() + b.square();
    EXPECT_EQ(lhs.to_string(), rhs.to_string());
}


TEST_F(GF281Test, TraceOfOne) {
    GF281 one = GF281::one();
    EXPECT_EQ(one.trace(), 1);
}

TEST_F(GF281Test, TraceLinearity) {
    GF281 a(randomBitString());
    GF281 b(randomBitString());
    int tr_sum = (a + b).trace();
    int sum_tr = (a.trace() + b.trace()) % 2;
    EXPECT_EQ(tr_sum, sum_tr);
}


TEST_F(GF281Test, MultiplicationWithZero) {
    GF281 a(randomBitString());
    GF281 zero = GF281::zero();

    GF281 res = a.multiply(zero, getLambda());
    EXPECT_EQ(res.to_string(), zero.to_string());
}

TEST_F(GF281Test, MultiplicationWithOne) {
    GF281 a(randomBitString());
    GF281 one = GF281::one();

    GF281 res = a.multiply(one, getLambda());
    EXPECT_EQ(res.to_string(), a.to_string());
}

TEST_F(GF281Test, MultiplicationDistributivity) {
    GF281 a(randomBitString());
    GF281 b(randomBitString());
    GF281 c(randomBitString());

    GF281 lhs = (a + b).multiply(c, getLambda());
    GF281 rhs = a.multiply(c, getLambda()) + b.multiply(c, getLambda());

    EXPECT_EQ(lhs.to_string(), rhs.to_string());
}


TEST_F(GF281Test, InverseIdentity) {
    GF281 a;
    a.setBit(0, 1); a.setBit(1, 1); a.setBit(50, 1);

    GF281 inv = a.inverse(getLambda());
    GF281 res = a.multiply(inv, getLambda());

    EXPECT_EQ(res.to_string(), GF281::one().to_string());
}

TEST_F(GF281Test, LabMethodologyCheck) {
    GF281 a(randomBitString());
    GF281 b(randomBitString());
    GF281 c(randomBitString());

    GF281 left = (a + b).multiply(c, getLambda());
    GF281 right = b.multiply(c, getLambda()) + c.multiply(a, getLambda());

    EXPECT_EQ(left.to_string(), right.to_string());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}