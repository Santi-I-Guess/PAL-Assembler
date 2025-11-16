from enum import Enum
import math
import random


# atom type
class Atom_T(Enum):
    LABEL = 1
    LITERAL_INT = 2
    LITERAL_STR = 3
    MNEMONIC = 4
    REGISTER = 5
    SOURCE = 6
    STACK_OFFSET = 7


# for eventually generating malformed programs
class Error_T(Enum):
    EXPECTED_MNEMONIC = 1,
    INVALID_ATOM = 1 << 1,
    MISSING_ARGUMENTS = 1 << 2,
    MISSING_EXIT = 1 << 3,
    MISSING_MAIN = 1 << 4,
    UNKNOWN_LABEL = 1 << 5,


# describe template of instruction
BLUEPRINTS_MAP = {
    "NOP":    [Atom_T.MNEMONIC],
    "CALL":   [Atom_T.MNEMONIC, Atom_T.LABEL],
    "RET":    [Atom_T.MNEMONIC],
    "PRINT":  [Atom_T.MNEMONIC, Atom_T.SOURCE],
    "SPRINT": [Atom_T.MNEMONIC, Atom_T.LITERAL_STR],
    "EXIT":   [Atom_T.MNEMONIC],
    "READ":   [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.LITERAL_INT],
    "WRITE":  [Atom_T.MNEMONIC, Atom_T.SOURCE,   Atom_T.LITERAL_INT],
    "POP":    [Atom_T.MNEMONIC, Atom_T.REGISTER],
    "PUSH":   [Atom_T.MNEMONIC, Atom_T.SOURCE],
    "MOV":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "ADD":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "SUB":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "MUL":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "DIV":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "AND":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "OR":     [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "XOR":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "LSH":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "RSH":    [Atom_T.MNEMONIC, Atom_T.REGISTER, Atom_T.SOURCE],
    "CMP":    [Atom_T.MNEMONIC, Atom_T.SOURCE,   Atom_T.SOURCE],
    "JEQ":    [Atom_T.MNEMONIC, Atom_T.LABEL],
    "JNE":    [Atom_T.MNEMONIC, Atom_T.LABEL],
    "JGE":    [Atom_T.MNEMONIC, Atom_T.LABEL],
    "JGR":    [Atom_T.MNEMONIC, Atom_T.LABEL],
    "JLE":    [Atom_T.MNEMONIC, Atom_T.LABEL],
    "JLS":    [Atom_T.MNEMONIC, Atom_T.LABEL],
}


# allows for random choosing
MNEMONICS_LIST = list(BLUEPRINTS_MAP.keys())


def gen_random_i16() -> int:
    # for readability, make small positive values more common than large values
    if random.randint(0, 3) == 0:
        return math.ceil(random.triangular(-2**15, 2**15))
    else:
        return math.ceil(random.triangular(-2**5, 2**6))


def gen_random_word() -> str:
    # nonsense, but somewhat easy to pronounce, with some variation
    consonants: str = "bcdfgjklprstvw"
    vowels: str = "aeiou"
    output_word: str = ""
    for i in range(4):
        if i % 2 == 0:
            given_char: str = random.choice(consonants)
            output_word += given_char
            if random.randint(0, 7) == 0 and given_char != "w":
                output_word += "w"
        else:
            given_char: str = random.choice(vowels)
            output_word += given_char
            if given_char in "io" and random.randint(0, 3) == 0:
                output_word += "a"
            if random.randint(0, 7) == 0:
                output_word += "n"
    return output_word


# to prevent scenario where random_word from SPRINT is the same as a label
# name. not a syntax error, but could be confusing
def gen_random_label() -> str:
    alphabet: str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    output_label: str = "."
    output_label += "".join(random.choice(alphabet) for i in range(4))
    output_label += f"_{random.randint(0, 99)}"
    return output_label


def gen_instruction() -> list[str]:
    global BLUEPRINTS_MAP
    mnemonic = random.choice(MNEMONICS_LIST)
    output_instruction = []
    curr_blueprint = BLUEPRINTS_MAP[mnemonic]
    for idx, curr_type in enumerate(curr_blueprint):
        if curr_type == Atom_T.LABEL:
            output_instruction.append(gen_random_label())
        elif curr_type == Atom_T.LITERAL_INT:
            rand_i16 = gen_random_i16()
            output_instruction.append(f"${rand_i16}")
        elif curr_type == Atom_T.LITERAL_STR:
            # print literal backslash, let final_project try to interpret that
            output_instruction.append(f"\"{gen_random_word()}\\n\"")
        elif curr_type == Atom_T.MNEMONIC:
            output_instruction.append(mnemonic)
        elif curr_type == Atom_T.REGISTER:
            output_instruction.append("R" + random.choice("ABCDEFGH"))
        elif curr_type == Atom_T.SOURCE:
            # LITERAL_INT, REGISTER, STACK_OFFSET
            the_choice = random.randint(0, 2)
            if the_choice == 0:
                # LITERAL_INT
                rand_i16 = gen_random_i16()
                output_instruction.append(f"${rand_i16}")
            elif the_choice == 1:
                # REGISTER
                output_instruction.append("R" + random.choice("ABCDEFGH"))
            else:
                # STACK_OFFSET
                output_instruction.append(f"%{random.randint(0, 4)}")
        elif curr_type == Atom_T.STACK_OFFSET:
            output_instruction.append(f"%{random.randint(0, 4)}")
    return output_instruction


def gen_raw_program() -> list[list[str]]:
    program_buffer = []
    for i in range(20):
        program_buffer.append(gen_instruction())
    if "EXIT" not in [i[0] for i in program_buffer]:
        program_buffer.append(["EXIT"])
    while program_buffer[0][0] == "EXIT":
        random.shuffle(program_buffer)
    return program_buffer


def insert_label_defs(program_buffer: list[list[str]]) -> list[list[str]]:
    new_program_buffer = program_buffer
    labels = ["main"]
    # look through instructions with label arguments
    for i in new_program_buffer:
        if i[0] not in ["CALL", "JEQ", "JNE", "JGE", "JGR", "JLE", "JLS"]:
            continue
        label_name = i[1]
        if label_name not in labels:
            labels.append(label_name)
    label_indexes = random.sample(
        range(2, len(new_program_buffer)),
        len(labels)
    )
    random.shuffle(labels)
    for idx, name in enumerate(labels):
        if idx == 0:
            new_program_buffer.insert(0, [f"{name}:"])
        else:
            new_program_buffer.insert(label_indexes[idx], [f"{name}:"])

    return new_program_buffer


def gen_erroneous_mnemonic() -> str:
    alphabet: str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    word_size: int = random.randint(3, 5)
    word: str = "".join(random.sample(alphabet, word_size))
    while word in MNEMONICS_LIST:
        word = "".join(random.sample(alphabet, word_size))
    return word


def gen_erroneous_atom() -> str:
    alphabet: str = "abcdefghijklmnopqrstuvwxyz"
    word: list[str] = random.sample(alphabet, 2) \
        + list(str(random.randint(10, 99)))
    random.shuffle(word)
    suffix_choice = random.randint(0, 6)
    if suffix_choice == 0:
        # might cause MISSING_ARGUMENTS instead of INVALID_ATOM
        word.insert(0, "\"")
        word.append("\"")
    elif suffix_choice == 1:
        word.insert(0, "$")
    elif suffix_choice == 2:
        word.insert(0, "%")
    return "".join(word)


def error_expected_mnemonic(program_buffer: list[list[str]]) \
                            -> list[list[str]]:
    # replace a random mnemonic with a nonexistant one
    error_line_idx: int = random.randint(1, len(program_buffer) - 1)
    while program_buffer[error_line_idx][0] in MNEMONICS_LIST:
        error_line_idx = random.randint(1, len(program_buffer) - 1)
    program_buffer[error_line_idx][0] = gen_erroneous_mnemonic()
    return program_buffer


def error_invalid_atom(program_buffer: list[list[str]]) -> list[list[str]]:
    # replace a random instruction argument with nonsense
    error_line_idx: int = random.randint(1, len(program_buffer) - 1)
    curr_ins: list[str] = program_buffer[error_line_idx]
    while curr_ins[0] not in MNEMONICS_LIST and len(curr_ins) < 2:
        error_line_idx = random.randint(1, len(program_buffer) - 1)
        curr_ins = program_buffer[error_line_idx]
    curr_ins[-1] = gen_erroneous_atom()
    program_buffer[error_line_idx] = curr_ins
    return program_buffer


def error_missing_arguments(program_buffer: list[list[str]]) \
                            -> list[list[str]]:
    # remove an argument of a random instruction
    error_line_idx: int = random.randint(1, len(program_buffer) - 1)
    curr_ins: list[str] = program_buffer[error_line_idx]
    # ensure curr_ins has an argument to remove
    while curr_ins[0] not in MNEMONICS_LIST or len(curr_ins) < 2:
        error_line_idx = random.randint(1, len(program_buffer) - 1)
        curr_ins = program_buffer[error_line_idx]
    curr_ins.pop()
    program_buffer[error_line_idx] = curr_ins
    return program_buffer


def error_missing_exit(program_buffer: list[list[str]]) \
                       -> list[list[str]]:
    # remove all instances of EXIT
    filtered_program_buffer = []
    for ins in program_buffer:
        if ins[0] != "EXIT":
            filtered_program_buffer.append(ins)
    return filtered_program_buffer


def error_missing_main(program_buffer: list[list[str]]) \
                       -> list[list[str]]:
    # remove instance of main label
    filtered_program_buffer = []
    for ins in program_buffer:
        if ins[0] != "main:":
            filtered_program_buffer.append(ins)
    return filtered_program_buffer


def error_unknown_label(program_buffer: list[list[str]]) \
                       -> list[list[str]]:
    # remove one random label definition
    label_idxs: list[int] = []
    for idx, ins in enumerate(program_buffer):
        if ins[0][-1] == ":" and ins[0] != "main:":
            label_idxs.append(idx)
    chosen_idx = random.choice(label_idxs)
    program_buffer.pop(chosen_idx)
    return program_buffer


def quick_check(program_buffer: list[list[str]]) -> None:
    if any(len(i) == 0 for i in program_buffer):
        print("program buffer has an empty element")
        for i in program_buffer:
            print(i, type(i))
        exit()
    if len(program_buffer) == 0:
        print("program buffer is empty")
        for i in program_buffer:
            print(" ".join(i))
        exit()
    if any(not isinstance(i, list) for i in program_buffer):
        print("something in program_buffer is not an object")
        for i in program_buffer:
            print(i, type(i))
        exit()


def insert_errors(program_buffer: list[list[str]],
                  error_bits: list[bool]) -> list[list[str]]:
    if error_bits[0]:  # EXPECTED_MNEMONIC
        quick_check(program_buffer)
        program_buffer = error_expected_mnemonic(program_buffer)
    if error_bits[1]:  # INVALID_ATOM
        quick_check(program_buffer)
        program_buffer = error_invalid_atom(program_buffer)
    if error_bits[2]:  # MISSING_ARGUMENTS
        quick_check(program_buffer)
        program_buffer = error_missing_arguments(program_buffer)
    if error_bits[3]:  # MISSING_EXIT
        quick_check(program_buffer)
        program_buffer = error_missing_exit(program_buffer)
    if error_bits[4]:  # MISSING_MAIN
        quick_check(program_buffer)
        program_buffer = error_missing_main(program_buffer)
    if error_bits[5]:  # UNKNOWN_LABEL
        quick_check(program_buffer)
        program_buffer = error_unknown_label(program_buffer)
    return program_buffer
