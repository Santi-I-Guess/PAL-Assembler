
# Why am I getting plus/minus 16383?
PAL uses the 14th, 13th, and 12th bit to determine the addressing mode of an
argument, and checks the addressing mode in the order of literal, stack,
string. To avoid strange unrepresentable ranges, all values are clamped
to plus/minus 2**14 - 1, which is 16383. To simulate overflow / underflow,
INC and DEC will clamp to the opposite sign of their value if the register
value goes outside \[-16383, 16383\]

# Why does entering nothing for INPUT push $10 to the stack?
INPUT is specifically for inputting integers, and if a letter is given, it
will be interpreted as the ascii value. To make the implementation consistant,
entering nothing will be interpreted as a newline, which is ascii value 10.

# Why is the result of this large multiplication unpredictable?
While operations of 3 digit numbers have been tested via a bash test script,
numbers that are greater than plus/minus 2**14 may have unexpected behavior.
If the operation would result in a number with more than 16 bits, the top
bits will get thrown away (due to c/c++ specification), and the 15th bit,
being the sign bit, may result in the proper sign value, or may not.

To counteract this, MUL, the main cause of this behavior, will check
the expected sign of the result with the actual sign, and change the
sign if the value needed to be clamped. This makes the value a tiny
bit more predictable, but it is recommended not to multiply by
arguments that cause values greater than an int16_t's range.

# Why are you opening the terminal device (/dev/tty and CONIN$) in the debugger?
In earlier iterations of the assembler (look for the commit that mentions
fixing piped input on Dec 4th), you'll notice that you can't really pipe
a program as input, then use the debugger without std::cin freaking out.
It's a bit of a strange issue, where stdin (fileno 0) will be set to eof
once it's done reading, which will be the end of the program, and subsequently
can't be used because stdin is still associated with the nameless pipe. This
causes std::cin to not be able to clear std::cin.eof and release control.

To skirt around this, the debugger will check if the input is from a pipe,
and if it is, it will open the controlling terminal device. On linux systems,
this is /dev/tty, and I think on windows it's CONIN$, but i'm not 100%
sure on that. This allows the debugger to take in user input while also
taking in piped input.

# How can I add my own instructions?
If you get bored and want to add your own instructions, there's just 4
steps you got to follow
1. Add the opcode, mnemonic, and blueprint to INS_BLUEPRINTS in
   src/common_values.h
2. create a function with your own implementation
3. add the function declaration to to instructions.h and put inside the
   cpu_handle class (in src/simulator/cpu_handle.h) as a public friend function
4. append your instruction to the if statement in cpu_handle::next_instruction
