#ifndef TRANSLATION_H
#define TRANSLATION_H 1

#include "common_values.h"

#include <cstdint>
#include <deque>
#include <map>
#include <vector>

// valid register symbols
const std::map<std::string, int16_t> REGISTER_TABLE = {
        {"RA", 0}, {"RB", 1}, {"RC", 2}, {"RD", 3},
        {"RE", 4}, {"RF", 5}, {"RG", 6}, {"RH", 7},
};

// opcode for each instruction mnemonic
const std::map<std::string, int16_t> OPCODE_TABLE = {
        {"NOP",    0}, {"CALL",  1}, {"RET",   2}, {"PRINT",  3},
        {"SPRINT", 4}, {"EXIT",  5}, {"READ",  6}, {"WRITE",  7},
        {"POP",    8}, {"PUSH",  9}, {"MOV",  10}, {"ADD",   11},
        {"SUB",   12}, {"MUL",  13}, {"DIV",  14}, {"AND",   15},
        {"OR",    16}, {"XOR",  17}, {"LSH",  18}, {"RSH",   19},
        {"CMP",   20}, {"JEQ",  21}, {"JNE",  22}, {"JGE",   23},
        {"JGR",   24}, {"JLE",  25}, {"JLS",  26},
};

// generates single token
int16_t translate_token(std::string token, Atom_Type atom_type,
                        std::map<std::string, int16_t> label_table);

// generates string bytes
std::deque<int16_t> translate_string(std::string token, int16_t &program_idx);

// generates the final program
Debug_Info generate_program(std::vector<int16_t> &result,
                            std::deque<std::string> tokens,
                            std::map<std::string, int16_t> label_table);

#endif
