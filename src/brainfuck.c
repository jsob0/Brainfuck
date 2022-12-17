#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <brainfuck.h>

int main(int argc, char **argv) {
    // The string stores the source code of brainfuck.
    char *source = NULL;

    // We start the iteration with a one, because we skip the first argument,
    // which is the name of this program.
    int i = 1;
    do {
        if (1 == argc || !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            printf(BF_message_help, argv[0]);
            return 0;
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            printf(BF_message_version, BRAINFUCK_VERSION);
            return 0;
        }
        else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
            // We need to check if the next argument, which is the output
            // path exists, because the user may not have typed it.
            if (argc < i + 1) {
                strcpy(brainfuck_path_output, "out.txt");
            }
            else {
                strcpy(brainfuck_path_output, argv[i + 1]);
            }
        }
        else if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--execute")) {
            // We need to check if the next argument, which is the source
            // code exists, because the user may not have typed it.
            if (argc < i + 1) {
                return 1;
            }

            // The user can use the execute argument many times, so we need
            // to release the previously allocated memory to avoid
            // memory leaks.
            if (source) {
                free(source);
            }

            // Copy the source code from the argument to the source variable.
            size_t source_len = strlen(argv[i + 1]);
            source = (char *)malloc(source_len + 1);
            if (NULL == source) {
                brainfuck_err_init("Failed to allocate memory space.\n");
                return 1;
            }

            strcpy(source, argv[i + 1]);
            strcpy(brainfuck_path_input, "console input");
		}
		++i;
    } while (i < argc);

	if (NULL == source) {
		// Set the path input if the user has not chose the execute argument.
		strcpy(brainfuck_path_input, argv[argc - 1]);
        source = brainfuck_read_file(brainfuck_path_input);
        if (NULL == source) return 1;
    }

	clock_t t;
	t = clock();	

    if (0 != brainfuck_interpret(source)) {
		t = clock() - t;
		double time_taken = (double)t / CLOCKS_PER_SEC;
		printf("\nThe program took %f seconds to execute.\n", time_taken);
	}

    free(source);
    return 0;
}

int brainfuck_interpret(char *source) {
    // Data for brainfuck code, filled with zeros and a pointer which points
    // on the current data.
    char *data = (char *)malloc(BRAINFUCK_DATA_SIZE);
    if (NULL == data) {
        brainfuck_err_init("Failed to allocate memory space.\n");
        return 1;
    }
    memset(data, 0, BRAINFUCK_DATA_SIZE);

    int ptr = 0, isError = 0, ip = 0;
    int source_len = strlen(source);

    // Open the output file if the user has set it.
	FILE *file_output = NULL;
	if (0 != *brainfuck_path_output) {
		file_output = fopen(brainfuck_path_output, "w");
		if (NULL == file_output) {
			brainfuck_err_init("Unable to open the output file `%s`\n",
					brainfuck_path_output);
			return 1;
		}
	}

    while (ip < source_len && !isError) {
        switch (source[ip]) {
        // Increment the data pointer.
        case BRAINFUCK_OP_NEXT:
            if (BRAINFUCK_DATA_SIZE + 1 == ++ptr) {
                brainfuck_err_code(ip, "Pointer index out of range (%d).\n",
                    BRAINFUCK_DATA_SIZE + 1);
                ++isError;
            }
            break;
        // Decrement the data pointer.
        case BRAINFUCK_OP_PREV:
            if (-1 == --ptr) {
                brainfuck_err_code(ip, "Pointer index out of range (-1).\n");
                ++isError;
            }
            break;
        // Increment the byte at the data pointer.
        case BRAINFUCK_OP_ADD:
            if (0 > ++data[ptr]) {
                brainfuck_err_code(ip, "Byte overflow (%d).\n",
                    BRAINFUCK_BYTE_SIZE + 1);
                ++isError;
            }
            break;
        // Decrement the byte at the data pointer.
        case BRAINFUCK_OP_SUB:
            if (0 > --data[ptr]) {
                brainfuck_err_code(ip, "Byte overflow (-1).\n");
                ++isError;
            }
            break;
        // Output the byte at the data pointer.
        case BRAINFUCK_OP_OUTPUT:
			fputc(data[ptr], (file_output) ? file_output : stdout);
            break;
        // Input a byte and store it at the data pointer.
        case BRAINFUCK_OP_INPUT:
            data[ptr] = getchar();
            break;
        // If the byte at the data pointer is 0, jump to the corresponding ']'
		case BRAINFUCK_OP_LOOP_START: {
            if (0 == data[ptr]) {
                int depth = 1, ipError = ip;
                while (depth) {
                    if (source_len == ++ip) {
                        brainfuck_err_code(ipError, "End of loop not found.\n");
                        ++isError;
                        break;
                    }

                    if ('[' == source[ip]) ++depth;
                    else if (']' == source[ip]) --depth;
                }
            }
        } break;
        // If the byte at the data pointer is not 0, jump to the
        // corresponding '['
        case BRAINFUCK_OP_LOOP_END: {
            if (0 != data[ptr]) {
                int depth = 1, ipError = ip;
                while (depth) {
                    if (-1 == --ip) {
                        brainfuck_err_code(ipError, "Start of loop not found.\n");
                        ++isError;
                        break;
                    }

                    if ('[' == source[ip]) --depth;
                    else if (']' == source[ip]) ++depth;
                }
            }
        } break;
        // The default statement is executed if no case constant-expression
        // value is equal to the value of expression.
        default:
            break;
        }
        ++ip;
    }

    if (file_output)
		fclose(file_output);
    free(data);
    return !isError;
}

char *brainfuck_read_file(const char *path) {
    // Open the file from the path in read mode and handle the error for it.
    FILE *file = NULL;
    file = fopen(path, "r");
    if (NULL == file) {
        brainfuck_err_init("Unable to open file `%s`\n", path);
        return NULL;
    }

    // Move the file cursor to the end of the file and get its position.
    fseek(file, 0, SEEK_END);
    size_t source_len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = NULL;
    source = (char *)malloc(source_len + 1);
    if (NULL == source) {
        brainfuck_err_init("Failed to allocate memory space.\n");
        fclose(file);
        return NULL;
    }

    // Read file content to the buffer.
    fread(source, 1, source_len, file);
    source[source_len] = 0;

    fclose(file);
    return source;
}

void brainfuck_err_init(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    // Error during initialization:
    // Unable to open file `hello.bf`
    fprintf(stderr, "Error during initialization:\n");
    vfprintf(stderr, fmt, ap);

    va_end(ap);
}

void brainfuck_err_code(int ch, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    // File "hello.bf", char 4
    // Error: Byte overflow (255)
    fprintf(stderr, "\nFile \"%s\", char %d\nError: ", brainfuck_path_input, ch);
    vfprintf(stderr, fmt, ap);

    va_end(ap);
}
