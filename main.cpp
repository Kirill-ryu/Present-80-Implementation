#include <iostream>
#include "header.hpp"
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <format>
typedef __int128_t int128;
using namespace std;

string hex128(unsigned __int128 n) {
    if (n == 0) return "0";
    string s = "";
    char hex_chars[] = "0123456789abcdef";
    while (n > 0) {
        s += hex_chars[n % 16];
        n /= 16;
    }
    reverse(s.begin(), s.end());
    return "0x" + s;
}

int main(){
    uint64_t plaintext = 0xFFFFFFFFFFFFFFFFULL;
    unsigned __int128 key = (unsigned __int128)0x00 << 64 | 0x00ULL;
    //std::cout<<"Plaintext : "<<std::hex<<std::showbase<<plaintext<<endl;
    //cout << "Key : " << hex128(key) << endl;


    const int iterations = 1000000;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; i++){
        Present80(plaintext, key);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Total Time : " << diff.count() << "s" << std::endl;
    std::cout << "Average Time per call : " << (diff.count() / iterations) << "s" << std::endl;

    return 0;
}