import random
import math

# generate many tests, then format into PAL language

# atom type
LABEL = 1
LITERAL_INT = 2
LITERAL_STR = 3
MNEMONIC = 4
REGISTER = 5
SOURCE = 6
STACK_OFFSET = 7


# describe template of instruction
# only the deterministic arithmetic instructions
BLUEPRINTS_MAP: dict[str, list] = {
    "ADD":    [MNEMONIC, REGISTER, SOURCE, SOURCE],
    "SUB":    [MNEMONIC, REGISTER, SOURCE, SOURCE],
    "MUL":    [MNEMONIC, REGISTER, SOURCE, SOURCE],
    "DIV":    [MNEMONIC, REGISTER, SOURCE, SOURCE]}
#    "AND":    [MNEMONIC, REGISTER, SOURCE, SOURCE],
#    "OR":     [MNEMONIC, REGISTER, SOURCE, SOURCE],
#    "NOT":    [MNEMONIC, REGISTER, SOURCE],
#    "XOR":    [MNEMONIC, REGISTER, SOURCE, SOURCE],
#    "LSH":    [MNEMONIC, REGISTER, SOURCE, SOURCE],
#    "RSH":    [MNEMONIC, REGISTER, SOURCE, SOURCE],

MNEMONICS_LIST = list(BLUEPRINTS_MAP.keys())

# only the general purpose registers
registers = ["RA", "RB", "RC", "RD",
             "RE", "RF", "RG", "RH"]


# pick random operation
# pick random, easy to visually verify functions
# simulate arithmetic logic
# print operation and RA

# note: since 1 << 15 is the sign bit, 1 << 14 is for literals, 1 << 13
#       is for string offsets, and 1 << 12 is for string, that means I only
#       have 13 bits for number range (since it checks for literals first,
#       we can use bits reserved for addressing in that case).

max_value = 16383
min_value = -16383

 
def clamp(result) -> int:
    if abs(result) < 1:
        result = 0
    elif result > max_value:
        return max_value
    elif result < min_value:
        return min_value
    return result


operation_map = {
    "ADD": "+",
    "SUB": "-",
    "MUL": "*",
    "DIV": "/",
}


if __name__ == "__main__":
    print("main:")
    for i in range(3):
        operation = random.choice(MNEMONICS_LIST)
        # arg_1 = int(random.triangular(min_value, max_value))
        # arg_2 = int(random.triangular(min_value, max_value))
        arg_1 = int(random.triangular(min_value, max_value))
        arg_2 = int(random.triangular(min_value, max_value))
        if operation == "ADD":
            result = arg_1 + arg_2
            result = clamp(result)
        elif operation == "SUB":
            result = arg_1 - arg_2
            result = clamp(result)
        elif operation == "MUL":
            result = arg_1 * arg_2
            # result_bin = bin(result)[2:]
            # result = int(result_bin[len(result_bin)-8:], 2)
            result = clamp(result)
        elif operation == "DIV":
            if arg_2 == 0:
                result = 1
            else:
                result = math.trunc(arg_1 / arg_2)
            if -1 < result and result < 1:
                result = 0

        result = clamp(result)
        print("    ; next operation")
        if operation == "NOT":
            arg_1 = f"${arg_1}"
            print(f"    {operation:<9} RA, {arg_1:>9}")
        else:
            arg_1 = f"${arg_1}"
            arg_2 = f"${arg_2}"
            print(f"    {operation:<9} RA, {arg_1:>9}, {arg_2:>9}")
        print(f"    SPRINT \"{arg_1:<9} {operation_map[operation]} {arg_2:<9} = ${result:<6}\"")
        print("    PUSH RA")
        print("    CALL arrow_print")
        # print("    check")
        # print("    PUSH RA")
        # print(f"    PUSH ${result}")
        # print("    CALL check_value")
    print("    EXIT")
    print("""arrow_print:
    ; %0 is the value calculated
    SPRINT \"  -> \"
    PRINT %0
    CPRINT $10
    POP RH ; dump parameter
    RET""")
    print("""\
check_value:
    ; %0 -> RA is expected, %0 -> RB is actual
    CMP %0, %1
    JEQ final
    SPRINT "expected "
    PRINT %0
    SPRINT ", got "
    PRINT %1
    CPRINT $10
final:
    POP RH ; dump parameters
    POP RH
    RET""")
