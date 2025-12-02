/* CSCI 200: Final Project: Pseudo Assembly Simulator
 *
 * Author: Santiago Sagastegui
 *
 * Pseudo assembly simulator for very simplified ISA. Also offers debug
 * flags, intermediate step files, and line-by-line printing.
 * Also hosted at https://github.com/Santi-I-Guess/final_project
 */

#include <vector>
#include <iostream>
#include <random>
#include <string>

#include "assembler/blueprint.h"
#include "assembler/tokenizer.h"
#include "assembler/translation.h"
#include "auxiliary.h"
#include "common_values.h"
#include "misc/cmd_line_opts.h"
#include "misc/file_handling.h"
#include "simulator/cpu_handle.h"

// every subdirectory of src is isolated in dependencies and function

std::string generate_file_header() {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> rand_letter(0, 25);
        std::string file_header = "";
        for (int i = 0; i < 10; ++i) {
                char curr_letter;
                int letter_idx = rand_letter(mt);
                curr_letter = (char)letter_idx + 'a';
                file_header += curr_letter;
        }
        return file_header;
}

int main(int argc, char **argv) {
        Cmd_Options life_opts;
        // TODO: split handle_cmd_args into store_args and is_valid_args
        bool valid_cmd_arg_combo = life_opts.handle_cmd_args(argc, argv);
        if (!valid_cmd_arg_combo)
                return 0;

        // produce random file header for intermediate files, which makes
        //      running multiple tests in a row unlikely to overwrite data
        std::string file_header = generate_file_header();

        // put assembled program here, so assembler module
        //      doesn't require cpu_handle
        std::vector<int16_t> final_program = {};
        if (life_opts.is_binary_input) {
                std::string bin_source_path = argv[life_opts.input_file_idx];
                populate_program_from_binary(final_program, bin_source_path);
        } else {
                // store user program into string
                std::string source_buffer = "";
                if (life_opts.input_file_idx != -1) {
                        std::string source_path = argv[life_opts.input_file_idx];
                        source_buffer = get_source_buffer(source_path, false);
                } else {
                        source_buffer = get_source_buffer("", true);
                }

                // Step 1: tokenize user program and define labels
                // create_label_map also removes label definitions from tokens
                // TODO: store associated enum for each token instead of
                //       quitting early, then check each enum for validitiy
                std::vector<std::string> tokens = create_tokens(source_buffer);
                std::map<std::string, int16_t> label_table = create_label_map(tokens);
                if (life_opts.intermediate_files)
                        generate_intermediates(file_header, tokens, label_table);

                // Step 2: perform grammar check on tokenized input
                // functions given handle_ prefix are capable of calling exit
                Debug_Info res;
                res = grammar_check(tokens, label_table);
                handle_grammar_check_res(res);

                // Step 3: assemble program
                Program_Info program_info = {};
                program_info.tokens = tokens;
                program_info.label_table = label_table;
                res = assemble_program(final_program, program_info);
                handle_assemble_res(res);
        }

        // write the assembled program to a binary file
        if (life_opts.assemble_only) {
                bool res_temp;
                res_temp = write_program_to_sink(final_program, file_header);
                if (!res_temp) {
                        std::cerr << "Failed to open token sink file\n";
                        return 1;
                }
                return 0;
        }

        CPU_Handle cpu_handle;
        cpu_handle.load_program(final_program);
        if (life_opts.is_debug)
                cpu_handle.run_program_debug();
        else
                cpu_handle.run_program();
        return 0;
}
