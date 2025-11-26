#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H 1

#include <deque>
#include <map>
#include <string>
#include <fstream>

/**
 * @brief generates token file and labels file
 * @details wrapper function of write_tokens_to_sink and write_labels_to_sink
 */
void generate_intermediates(std::string file_header,
                            std::deque<std::string> tokens,
                            std::map<std::string, int16_t> label_table);

/**
 * @brief populates final program from input if -b flag is given
 */
void populate_program_from_binary(std::deque<int16_t> &program, std::string filepath);

/**
 * @brief reads ascii source file into buffer
 * @details istream instead of ifstream to ensure std::cin works as in input
 */
std::string read_file_to_buffer(std::ifstream &source_file);

/**
 * @brief writes intermediate file for label
 */
bool write_labels_to_sink(std::map<std::string, int16_t> label_table, std::string header);

/**
 * @brief writes intermediate file for assembled progra
 */
bool write_program_to_sink(std::deque<int16_t> program, std::string header);

/**
 * @brief writes intermediate file for token
 */
bool write_tokens_to_sink(std::deque<std::string> tokens, std::string header);

#endif
