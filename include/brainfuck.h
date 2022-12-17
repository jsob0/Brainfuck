#ifndef BRAINFUCK_H
#define BRAINFUCK_H

// The current version of the program.
#define BRAINFUCK_VERSION "1.1"

// The maximum size of the data on which the user can operate.
#define BRAINFUCK_DATA_SIZE 30000

// The maximum size of a single byte in the data.
#define BRAINFUCK_BYTE_SIZE 127

/* Brainfuck opeartors */

#define BRAINFUCK_OP_NEXT '>'
#define BRAINFUCK_OP_PREV '<'
#define BRAINFUCK_OP_ADD '+'
#define BRAINFUCK_OP_SUB '-'
#define BRAINFUCK_OP_OUTPUT '.'
#define BRAINFUCK_OP_INPUT ','
#define BRAINFUCK_OP_LOOP_START '['
#define BRAINFUCK_OP_LOOP_END ']'

// The path from which the file will be readed.
char brainfuck_path_input[260];

// The path where the output message will be saved.
char brainfuck_path_output[260];

/**
 * Interpret and execute brainfuck code from the passed source.
 *
 * @param source The string storing brainfuck code to interpretation and
 *      execution.
 *
 * @returns If the function succeeds, the return value is nonzero.
 */
int brainfuck_interpret(char *source);

/**
 * Read brainfuck code from the passed path.
 *
 * @param path The path of the file from which the file will be read.
 *
 * @returns Dynamically allocated string with brainfuck code.
 */
char *brainfuck_read_file(const char *path);

/**
 * Print in the console information about the error that occurred in
 * the brainfuck code.
 *
 * @param ch The number of the character in which the error occurred.
 * @param fmt A printf() style message format string.
 * @param ... Additional parameters matching % tokens in the fmt string,
 *      if any.
 */
void brainfuck_err_code(int ch, const char *fmt, ...);

/**
 * Print in the console information about the error that occurred during
 * the initialization of the interpreter.
 *
 * @param ch The number of the character in which the error occurred.
 * @param fmt A printf() style message format string.
 * @param ... Additional parameters matching % tokens in the fmt string,
 *      if any.
 */
void brainfuck_err_init(const char *fmt, ...);

/* Messages for user */

const char *BF_message_help = "\
Usage: %s [OPTIONS] [FILE/-e CODE]\n\
\n\
Interpret brainfuck code by passing the file or the code itself.\n\
\n\
Available options:\n\
  -h, --help          Show this help message.\n\
  -v, --version       Show the program version.\n\
  -o, --output FILE   Write output to FILE.\n\
  -e, --execute CODE  Execute the code specified by CODE.\n\
\n\
For more information or to report bugs, add me on discord sopa#7147\n\
";

const char *BF_message_version = "\
Brainfuck %s version.\n\
\n\
Copyright (c) 2022 Jakub Sobczak.\n\
Distributed under the MIT License.\n\
";

#endif /* BRAINFUCK_H */
