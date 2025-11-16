#!/usr/bin/env bash

set -o nounset

# assuming project directory is named "final_project"

# get relative paths to important files
project_root=$(realpath --relative-to=./ $(pwd | sed "s/\(.*final_project\).*/\1/"))
testing_dir=$(realpath --relative-to=./ "${project_root}/testing")
test_gen=$(realpath --relative-to=./ ${testing_dir}/test_generation.py)
program=$(realpath --relative-to=./ "${project_root}/final_project")

if [[ ! -f ${project_root}/final_project ]]; then
    printf "warning: project executable could not be found. exiting...\n"
    exit
fi

for i in ${testing_dir}/example_programs/*; do
    rm $i
done
python3 ${test_gen}

for i in ${testing_dir}/example_programs/gib_valid_*; do
    printf "\x1b[32m${i}\x1b[0m:\n"
    ${program} ${i}
done

for i in ${testing_dir}/example_programs/gib_error_*; do
    printf "\x1b[31m${i}\x1b[0m:\n"
    ${program} ${i}
done
