
# Why am I getting plus/minus 16384?
PAL uses the 14th, 13th, and 12th bit to determine the addressing mode of an
argument, and checks the addressing mode in the order of literal, stack,
string. To avoid strange unrepresentable ranges, all values are clamped
to plus/minus 2**12 - 1, which is 16383. To simulate overflow / underflow,
INC and DEC will clamp to the opposite sign of their value if the register
value goes outside \[-16384, 16384\]

# Why does entering nothing for INPUT push $10?
INPUT is specifically for inputting integers, and if a letter is given, it
will be interpreted as the ascii value. To make the implementation consistant,
entering nothing will be interpreted as a newline, which is ascii value 10.


