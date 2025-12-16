#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <functional>
#include "GF281.h"

using namespace std;

void benchmark(string operationName, int iterations, std::function<void()> func) {
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; i++) {

        func();
    }

    auto end = chrono::high_resolution_clock::now();

    double totalTimeNs = (double)chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    double avgTimeNs = totalTimeNs / iterations;

    cout << "| " << left << setw(20) << operationName
        << " | " << right << setw(15) << iterations
        << " | " << right << setw(15) << fixed << setprecision(2) << avgTimeNs << " ns |" << endl;
}

int main() {
    cout << "Lab 4. V8. m = 281." << endl;

    auto lambda = GF281::computeLambdaMatrix();
    cout << "Done." << endl << endl;

    GF281 a("1101101010110001");
    GF281 b("1010010001010101");

    cout << "Performance Results (Average Time per Operation):" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "| Operation            | Iterations      | Avg Time (ns)   |" << endl;
    cout << "-------------------------------------------------------------" << endl;

    benchmark("Add (+)", 10000, [&]() {
        GF281 res = a + b;
        (void)res;
        });

    benchmark("Square (^2)", 10000, [&]() {
        GF281 res = a.square();
        (void)res;
        });

    benchmark("Trace (tr)", 10000, [&]() {
        int t = a.trace();
        (void)t;
        });

    benchmark("Multiply (*)", 100, [&]() {
        GF281 res = a.multiply(b, lambda);
        (void)res;
        });

    benchmark("Inverse (^-1)", 2, [&]() {
        GF281 res = a.inverse(lambda);
        (void)res;
        });

    benchmark("Power (a^100)", 10, [&]() {
        GF281 res = a.power("1100100", lambda);
        (void)res;
        });

    cout << "-------------------------------------------------------------" << endl;
    return 0;
}