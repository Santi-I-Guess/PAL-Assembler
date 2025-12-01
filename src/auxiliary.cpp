#include <cstdlib>
#include <iostream>

#include "auxiliary.h"
#include "common_values.h"

// refactoring functions for main.cpp, therefore i'll allow includes from
// other modules here

void handle_assemble_res(Debug_Info res) {
        switch (res.assembler_retval) {
        case ACCEPTABLE_2:
                break;
                std::exit(0);
        case EXPECTED_MNEMONIC_2:
                std::cerr << "--- Assembler Error ---\n";
                std::cerr << "Mnemonic is undefined\n";
                std::cerr << "Instruction #" << res.relevant_idx << "\n";
                std::cerr << "Invalid mnemonic: " << res.relevant_tokens.at(0) << "\n";
                std::exit(0);
        case INVALID_ATOM_2:
                std::cerr << "--- Assembler Error ---\n";
                std::cerr << "Invalid Atom\n";
                std::cerr << "Instruction #" << res.relevant_idx << "\n";
                std::cerr << "Invalid token: " << res.relevant_tokens.at(0) << "\n";
                std::exit(0);
        case MISSING_ARGUMENTS_2:
                // is intentionally duplicate, will remove the previous
                // MISSING_ARGUMENTS later
                std::cerr << "--- Assembler Error ---\n";
                std::cerr << "Missing arguments\n";
                std::cerr << "Instruction #" << res.relevant_idx << "\n";
                std::cerr << "Mnemonic: " << res.relevant_tokens.at(0) << "\n";
                std::exit(0);
        case MISSING_MAIN_2:
                std::cerr << "--- Assembler Error ---\n";
                std::cerr << "Main label was never defined\n";
                std::exit(0);
        case UNKNOWN_LABEL_2:
                std::cerr << "--- Assembler Error ---\n";
                std::cerr << "Label is undefined\n";
                std::cerr << "Token #" << res.relevant_idx << "\n";
                std::cerr << "Invalid instruction: ";
                std::cerr << res.relevant_tokens.at(0) << " ";
                std::cerr << res.relevant_tokens.at(1) << "\n";
        }
}

void handle_grammar_check_res(Debug_Info res) {
        switch (res.grammar_retval) {
        case ACCEPTABLE:
                break;
        case EXPECTED_MNEMONIC:
                std::cerr << "--- Grammar Error ---\n";
                std::cerr << "Expected mnemonic\n";
                std::cerr << "Instruction #" << res.relevant_idx << "\n";
                std::cerr << "Actual Symbol: " << res.relevant_tokens.at(0) << "\n";
                std::exit(0);
        case INVALID_ATOM:
                std::cerr << "--- Grammar Error ---\n";
                std::cerr << "Invalid atom\n";
                std::cerr << "Instruction #" << res.relevant_idx << "\n";
                std::cerr << "Mnemonic: " << res.relevant_tokens.at(0) << "\n";
                std::cerr << "Invalid symbol: " << res.relevant_tokens.at(1) << "\n";
                std::exit(0);
        case MISSING_ARGUMENTS:
                std::cerr << "--- Grammar Error ---\n";
                std::cerr << "Missing arguments\n";
                std::cerr << "Instruction #" << res.relevant_idx << "\n";
                std::cerr << "Mnemonic: " << res.relevant_tokens.at(0) << "\n";
                std::exit(0);
        case MISSING_EXIT:
                std::cerr << "--- Grammar Error ---\n";
                std::cerr << "Missing EXIT instruction\n";
                std::exit(0);
        case MISSING_MAIN:
                std::cerr << "--- Grammar Error ---\n";
                std::cerr << "Missing MAIN label\n";
                std::exit(0);
        case UNKNOWN_LABEL:
                std::cerr << "--- Grammar Error ---\n";
                std::cerr << "Attempted to call unknown label\n";
                std::cerr << "Instruction #" << res.relevant_idx << "\n";
                std::cerr << "Mnemonic: " << res.relevant_tokens.at(0) << "\n";
                std::cerr << "Invalid symbol: " << res.relevant_tokens.at(1) << "\n";
                std::exit(0);
        }
}
