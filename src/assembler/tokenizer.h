#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <cstddef>
#include <vector>
#include <string>

#include "../common_values.h"

/**
 * @brief tokenizes the user input, and associates types to each token
 */
std::vector<Token> create_tokens(const std::string source_buffer);

/**
 * @brief checks if i matches equivalent regex pattern: [$%|a-zA-Z0-9_-]
 * @details create_tokens helper function
 */
bool is_identifier_char(const char i);

#endif
