#!/usr/bin/env bash

set -o nounset

# using SPRINT and PRINT instruction to do my testing

print_check() {
    printf "\x1b[32mPrint Check:\x1b[0m\n"
    printf "\x1b[32mExpect: 1-50, one per line\x1b[0m\n"
    program=$(printf "main:
        MOV RA, \$1
        again:
            PRINT RA
            CPRINT \$10
            INC RA
            CMP RA, \$50
            JLE again
            EXIT")
    printf "%s\n" "$program" | ../final_project
}

read_write_check() {
    # check READ and WRITE
    printf "\x1b[32mREAD-WRITE Check:\x1b[0m\n"
    printf "\x1b[32mExpect: 0-100, one per line, skip 2's\x1b[0m\n"
    program=$(printf "\
        main:
        MOV RB, \$0
        again:
            WRITE RA, RB
            READ RA, RB
            PRINT RA
            CPRINT \$10
            INC RB
            INC RA
            LSH RA, RB, \$1
            CMP RB, \$51
            JLS again
        ;
        EXIT")
    printf "%s\n" "$program" | ../final_project
}

basic_loop_check() {
    # check PUSH, POP, CMP, JLS, and stack offset addressing
    printf "\x1b[32mBasic Loop Check:\x1b[0m\n"
    printf "\x1b[32mExpect: 12-1, four per line\x1b[0m\n"
    escaped_n="n"
    program="\
        print_top_4:
            PRINT %0
            SPRINT \" \"
            PRINT %1
            SPRINT \" \"
            PRINT %2
            SPRINT \" \"
            PRINT %3
            SPRINT \"\\${escaped_n}\"
            POP RB
            POP RB
            POP RB
            POP RB
            RET
        main:
            MOV RA, \$1
        again_1:
            PUSH RA
            INC RA
            CMP RA, \$12
            JLE again_1
        MOV RA, \$0
        again_2:
            CALL print_top_4
            INC RA
            CMP RA, \$3
            JLS again_2
        EXIT
    "
    printf "%s\n" "$program" | ../final_project
}

ascii_check() {
    # check CPRINT
    printf "\x1b[32mAscii Check:\x1b[0m\n"
    printf "\x1b[32mExpect: lowercase a-z, then uppercase A-Z, on seperate lines\x1b[0m\n"
    escaped_n="n"
    program="\
        print_alphabet:
        ; RA is the value, RB is the bounds
        POP RA
        MOV RB, RA
        ADD RB, RB, \$26
        again_1:
        CPRINT RA
        INC RA
        CMP RA, RB
        JGE next_1
        SPRINT \" \"
        JLS again_1
        next_1:
        SPRINT \"\\${escaped_n}\"
        RET
        ;
        main:
        PUSH \$97
        CALL print_alphabet
        PUSH \$65
        CALL print_alphabet
        EXIT
    "
    printf "%s\n" "${program}" | ../final_project
}

arithmetic_check() {
    printf "\x1b[32mArithmetic Check:\x1b[0m\n"
    printf "\x1b[32mExpect: Multiplication Table for 2-99\x1b[0m\n"
    program="\
    ; somehow opcode is corrupted at RSP 515
    align_right:
        ; assume 1-3 digits
        CMP %0, \$10    JLS one_digit
        CMP %0, \$100   JLS two_digits
        JMP align_right_final
    one_digit:
        CPRINT \$32
        CPRINT \$32
        JMP align_right_final
    two_digits:
        CPRINT \$32
    JMP align_right_final
    align_right_final:
        PRINT %0
        POP RA ; dump parameter
        RET
        ;
    print_row:
        ; RA as current multiple value, RB as ctr
        MOV RB, \$1
        print_loop:
        MUL RA, %0, RB
        PUSH RA
        CALL align_right
        CPRINT \$32
        INC RB
        CMP RB, \$11
        JLS print_loop
        CPRINT \$10
        RET
        ;
    main:
        ; RA as both value and ctr
        MOV RA, \$2
        main_loop:
        PUSH RA
        CALL print_row
        POP RA
        INC RA
        CMP RA, \$100
        JLS main_loop
        JLS main_loop
        EXIT
        EXIT
    "
    printf "%s\n" "${program}" | ../final_project
}

tests=(
    print_check
    read_write_check
    basic_loop_check
    ascii_check
    arithmetic_check
)


if [[ "${#}" -ne 1 ]]; then
    printf "usage: ./test_suite.sh <index|\"all\">\n"
    exit
fi

if [[ ! "${1}" =~ ^[0-9]+$ ]]; then
    if [[ "${1}" == "all" ]]; then
        ${tests[0]}
        ${tests[1]}
        ${tests[2]}
        ${tests[3]}
        ${tests[4]}
    else
        printf "non-digit argument is not \"all\"\n"
    fi
elif [[ ${1} -eq 1 ]]; then
    ${tests[0]}
elif [[ ${1} -eq 2 ]]; then
    ${tests[1]}
elif [[ ${1} -eq 3 ]]; then
    ${tests[2]}
elif [[ ${1} -eq 4 ]]; then
    ${tests[3]}
elif [[ ${1} -eq 5 ]]; then
    ${tests[4]}
fi
