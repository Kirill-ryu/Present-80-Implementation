#include <iostream>
#include "header.hpp"
#include <cstdint>
#include <algorithm>
#include <iomanip>

typedef __int128_t int128;


// Generate round key.
// Extract upper 64bit from key.
std::uint64_t generateRoundKey(unsigned __int128 key){
    return (uint64_t)(key >> 16);
}

// XOR state with round key.
// Perform XOR between state and round key from generateRoundKey().
std::uint64_t addRoundKey(std::uint64_t state, std::uint64_t RoundKey ){
    std::uint64_t newstate = 0;
    newstate |= state ^ RoundKey;

    return newstate;
}

// This is a table for sBox substitution.
const uint8_t sBOX_table[16] = {
     0xC, 0x5, 0x6, 0xB,
     0x9, 0x0, 0xA, 0xD,
     0x3, 0xE, 0xF, 0x8,
     0x4, 0x7, 0x1, 0x2};

// sBox layer.
// divide into 16 4bit blocks, and substitute the values by sBOX_table.
std::uint64_t sBox(std::uint64_t state){
    std::uint64_t newState = 0;
    for (int i=0; i<16; i++){
        std::uint8_t temp1 = (state>>(4*i)) & ((1<<4)-1);
        std::uint8_t temp2 = sBOX_table[temp1];
        newState |= (std::uint64_t)temp2 << (4*i);
    }
    return newState;
}

// A table for pLayer repositioning.
const uint64_t pLayer_table[64] ={
     0x0, 0x10, 0x20, 0x30,
     0x1, 0x11, 0x21, 0x31, 
     0x2, 0x12, 0x22, 0x32, 
     0x3, 0x13, 0x23, 0x33, 
     0x4, 0x14, 0x24, 0x34, 
     0x5, 0x15, 0x25, 0x35, 
     0x6, 0x16, 0x26, 0x36, 
     0x7, 0x17, 0x27, 0x37, 
     0x8, 0x18, 0x28, 0x38, 
     0x9, 0x19, 0x29, 0x39, 
     0xA, 0x1A, 0x2A, 0x3A, 
     0xB, 0x1B, 0x2B, 0x3B, 
     0xC, 0x1C, 0x2C, 0x3C, 
     0xD, 0x1D, 0x2D, 0x3D, 
     0xE, 0x1E, 0x2E, 0x3E, 
     0xF, 0x1F, 0x2F, 0x3F
    };

// p Layer.
// reposition each bit position according to pLayer_table.
std::uint64_t pLayer(std::uint64_t state){
    std::uint64_t newstate = 0;
    for (int i=0; i<64; i++){
        std::uint64_t temp = (state>>i) & 1;
        newstate |= temp << pLayer_table[i];
    }
    return newstate;
}

// 19 bit right rotate the round key.
unsigned __int128 Rotation(unsigned __int128 key){
    unsigned __int128 newkey = 0;
    unsigned __int128 high_61 = 0;
    unsigned __int128 low_19 = 0;
    high_61 = (key >> 19);
    low_19 = (key & 0x7FFFF);
    newkey |= ( low_19 << 61 ) | high_61;

    return newkey;
}

// Perform sBox substitution on leftmost 4 bits.
unsigned __int128 Substitution(unsigned __int128 key){
    std::uint64_t temp = 0;
    temp = (key >> 76) & 0xF;
    temp = sBox(temp) & 0xF;
    
    key &= ~((unsigned __int128)(0xFULL) << 76);
    key |= ((unsigned __int128)temp << 76);

    return key;
}

// XOR state with specific bits(k15~k19) 0f round key.
unsigned __int128 CounterXOR(unsigned __int128 key, int round){
    unsigned __int128 temp1 = 0;
    unsigned __int128 round2 = static_cast<unsigned __int128>(round);
    unsigned __int128 temp2 = 0;
    temp1 = (key >> 15) & 0x1F;
    temp2 = (temp1 ^ round2) & 0x1F;
    key &= ~((unsigned __int128)(0x1F << 15));
    key |= temp2 << 15;

    return key;
}

// apply rotation, sBox substitution, and round counter XOR
unsigned __int128 KeyUpdate(__uint128_t key, int round){
    unsigned __int128 newkey = 0;
    newkey = Rotation(key);
    newkey = Substitution(newkey);
    newkey = CounterXOR(newkey, round);
    return newkey;
}

// Perform whole Present-80 process.
void Present80(uint64_t state, unsigned __int128 key){

    for (int i = 1; i <= 31; i++){  // generateRoundkey - Sbox - pLayer - Keyupdate for 31 round.
        uint64_t roundKey = generateRoundKey(key);
        state = addRoundKey(state, roundKey);

        state = sBox(state);

        state = pLayer(state);

        key = KeyUpdate(key, i);
    }
    uint64_t finalRoundKey = generateRoundKey(key);   // Final Whitening & finish
    state = addRoundKey(state, finalRoundKey);

    std::cout<<"Ciphertext:" << std::hex << std::setw(16) << std::setfill('0') << (uint64_t)state << std::endl;
}