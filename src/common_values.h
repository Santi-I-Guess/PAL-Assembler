#ifndef COMMON_VALUES_H
#define COMMON_VALUES_H 1

#include <vector>
#include <string>
#include <map>

// stack size last 1/4 of the RAM
#define RAM_SIZE 3000
#define STACK_START 2250
#define STACK_SIZE 750
#define CALL_STACK_SIZE 2014
#define LIT_MIN_VALUE -16383
#define LIT_MAX_VALUE 16383

/**
 * @brief enum for atom (a.k.a. argument) type
 */
enum Atom_Type {
        LABEL,
        LITERAL_INT,
        LITERAL_STR,
        MNEMONIC,
        REGISTER,
        SOURCE,       // LITERAL_INT | REGISTER | STACK_OFFSET
        STACK_OFFSET,
};

/**
 * @brief defines possible token types
 */
enum Token_Type {
        T_INTEGER_LIT,
        T_LABEL_DEF,
        T_LABEL_REF,
        T_MNEMONIC,
        T_REGISTER,
        T_STACK_OFF,
        T_STRING_LIT,
};

/**
 * @brief struct that associates original line number and token type
 * to a string token
 */
struct Token {
        int line_num;     ///< line number from original input
        Token_Type type;  ///< type of the token
        std::string data; ///< the data
};

/**
 * @brief stores all relevant information of an instruction in one place
 * @details helper struct for INS_BLUEPRINTS
 */
struct Instruction_Data {
        size_t length;
        int16_t opcode;
        std::string mnem_name;
        std::vector<Atom_Type> blueprint;
        Instruction_Data() = default;
        Instruction_Data(
                int16_t given_opcode,
                std::string given_mnem_name,
                std::vector<Atom_Type> given_blueprint
        );
};

/**
 * @brief hashmap that defines template of instructions in assembly language
 */
const std::map<std::string, Instruction_Data> INS_BLUEPRINTS = {
        {"NOP",    Instruction_Data(0, "NOP",     {MNEMONIC})},
        {"MOV",    Instruction_Data(1, "MOV",     {MNEMONIC, REGISTER, SOURCE})},
        {"INC",    Instruction_Data(2, "INC",     {MNEMONIC, REGISTER})},
        {"DEC",    Instruction_Data(3, "DEC",     {MNEMONIC, REGISTER})},
        {"ADD",    Instruction_Data(4, "ADD",     {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"SUB",    Instruction_Data(5, "SUB",     {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"MUL",    Instruction_Data(6, "MUL",     {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"DIV",    Instruction_Data(7, "DIV",     {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"MOD",    Instruction_Data(8, "MOD",     {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"AND",    Instruction_Data(9, "AND",     {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"OR",     Instruction_Data(10, "OR",     {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"NOT",    Instruction_Data(11, "NOT",    {MNEMONIC, REGISTER, SOURCE})},
        {"XOR",    Instruction_Data(12, "XOR",    {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"LSH",    Instruction_Data(13, "LSH",    {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"RSH",    Instruction_Data(14, "RSH",    {MNEMONIC, REGISTER, SOURCE, SOURCE})},
        {"CMP",    Instruction_Data(15, "CMP",    {MNEMONIC, SOURCE,   SOURCE})},
        {"JMP",    Instruction_Data(16, "JMP",    {MNEMONIC, LABEL})},
        {"JEQ",    Instruction_Data(17, "JEQ",    {MNEMONIC, LABEL})},
        {"JNE",    Instruction_Data(18, "JNE",    {MNEMONIC, LABEL})},
        {"JGE",    Instruction_Data(19, "JGE",    {MNEMONIC, LABEL})},
        {"JGR",    Instruction_Data(20, "JGR",    {MNEMONIC, LABEL})},
        {"JLE",    Instruction_Data(21, "JLE",    {MNEMONIC, LABEL})},
        {"JLS",    Instruction_Data(22, "JLS",    {MNEMONIC, LABEL})},
        {"CALL",   Instruction_Data(23, "CALL",   {MNEMONIC, LABEL})},
        {"RET",    Instruction_Data(24, "RET",    {MNEMONIC})},
        {"PUSH",   Instruction_Data(25, "PUSH",   {MNEMONIC, SOURCE})},
        {"POP",    Instruction_Data(26, "POP",    {MNEMONIC, REGISTER})},
        {"WRITE",  Instruction_Data(27, "WRITE",  {MNEMONIC, SOURCE,   SOURCE})},
        {"READ",   Instruction_Data(28, "READ",   {MNEMONIC, REGISTER, SOURCE})},
        {"PRINT",  Instruction_Data(29, "PRINT",  {MNEMONIC, SOURCE})},
        {"SPRINT", Instruction_Data(30, "SPRINT", {MNEMONIC, LITERAL_STR})},
        {"CPRINT", Instruction_Data(31, "CPRINT", {MNEMONIC, SOURCE})},
        {"INPUT",  Instruction_Data(32, "INPUT",  {MNEMONIC})},
        {"SINPUT", Instruction_Data(33, "INPUT",  {MNEMONIC})},
        {"RAND",   Instruction_Data(34, "RAND",   {MNEMONIC})},
        {"EXIT",   Instruction_Data(35, "EXIT",   {MNEMONIC})},
};

/**
 * @brief hashmap for valid callable registers in assembly language
 */
const std::map<std::string, int16_t> REGISTER_TABLE = {
        {"RA",  0}, {"RB",  1}, {"RC",    2}, {"RD",    3},
        {"RE",  4}, {"RF",  5}, {"RG",    6}, {"RH",    7},
        {"RSP", 8}, {"RIP", 9}, {"CMP0", 10}, {"CMP1", 11}
};


Instruction_Data get_instruction(const int16_t &opcode);

std::string get_mnem_name(const int16_t &opcode);

int16_t get_opcode(const std::string &mnem_name);

#endif
