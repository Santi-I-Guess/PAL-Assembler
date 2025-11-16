#!/usr/bin/env bash

rm example_programs/*
python3 test_generation.py

# correct_g=0
# incorrect_g=0
# correct_e=0
# incorrect_e=0

declare -a incorrect_files
for i in example_programs/gib_valid_*; do
    printf "\x1b[32m${i}\x1b[0m:\n"
    ../final_project ${i}
done

for i in example_programs/gib_error_*; do
    printf "\x1b[31m${i}\x1b[0m:\n"
    ../final_project ${i}
done

# echo "correct valid programs: ${correct_g}"
# echo "incorrect valid programs: ${incorrect_g}"
# echo "correct erroneous programs: ${correct_e}"
# echo "incorrect erroneous programs: ${incorrect_e}"
# if [[ ${#incorrect_files[@]} -ne 0 ]]; then
#     echo "incorrect files:  ${arr_name[@]}"
# fi
