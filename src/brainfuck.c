/* Brainfuck interpreter ( https://github.com/jsob0/Brainfuck ) by jsob0. */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#define VERSION "1.0.0"

// The maximum size of the data on which the user can operate
#define MEM_MAX_SIZE 30000

// The maximum size of a single byte in the data
#define BYTE_MAX_SIZE 255

// Variable for error handling, so that we do not have to specify each time
// the function path to the compiling file.
char bf_compile_filepath[128] = { 0 };

/*
 * Compile the brainfuck code.
 *
 * \param source The string with the code to compile.
 *
 * \returns If the compilation succeeds, the return value is nonzero.
 */
int compile(char *source);

/*
 * Read the brainfuck code from the specified file.
 * 
 * \param filepath The path to the file to read.
 * 
 * \returns Dynamically allocated string with the code or NULL if the
 *			read fails
 */
char *read_code(const char *filepath);

/*
 * Error handling, write in the console information about the error and in
 * which character it was.
 *
 * \param ch The number of the character in which the error occurred.
 * \param fmt A printf() style message format string.
 * \param ... Additional parameters matching % tokens in the fmt string,
 *            if any.
 * 
 * Example message:
 * 
 * File "C:\users\user\hello_world.bf", char 43
 * Error: Byte overflow (255)
 */
void errprintf(int ch, const char *fmt, ...);

int main(int argc, char **argv) {
	if (2 != argc ||
		strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "usage: %s [-vh] / [file...]\n", argv[0]);
		printf("\t-v --version\tshow interpreter version.\n");
		printf("\t-h --help\tshow help menu.\n");
		return 1;
	}
	
	if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
		printf("brainfuck %s version.\n", VERSION);
		printf("Copyright (c) 2022 Jakub Sobczak.\n");
		printf("Distributed under the MIT License.\n");
		return 0;
	}

	strcpy(bf_compile_filepath, argv[1]);

	char *source = read_code(bf_compile_filepath);
	if (NULL == source) {
		// error handling in the function
		return 1;
	} /* If the read fails */

	clock_t t;
	t = clock();

	if (0 != compile(source)) {
		t = clock() - t;
		double time_taken = ((double)t) / CLOCKS_PER_SEC;
		printf("\nThe program took %f seconds to execute.\n", time_taken);
	} /* If the compilation succeeds */

	free(source);
	return 0;
}

int compile(char *source) {
	// Data for brainfuck code, filled with zeros and a pointer which points
	// on the current data.
	unsigned char *data = (unsigned char *)malloc(MEM_MAX_SIZE);
	if (NULL == data) {
		fprintf(stderr, "Failed to allocate memory for a data.\n");
		return 1;
	} /* if failed to allocate memory for a data */
	memset(data, 0, MEM_MAX_SIZE);

	int ptr = 0, isError = 0, ip = 0;
	int source_len = strlen(source);

	while (ip < source_len && !isError) {
		switch (source[ip]) {
		// Increment the data pointer.
		case '>':
			if (MEM_MAX_SIZE == ++ptr) {
				errprintf(ip, "Pointer index out of range (%d).\n",
					MEM_MAX_SIZE + 1);
				++isError;
			}
			break;
		// Decrement the data pointer.
		case '<':
			if (-1 == --ptr) {
				errprintf(ip, "Pointer index out of range (-1).\n");
				++isError;
			}
			break;
		// Increment the byte at the data pointer.
		case '+':
			if (BYTE_MAX_SIZE == ++data[ptr]) {
				errprintf(ip, "Byte overflow (%d).\n", BYTE_MAX_SIZE + 1);
				++isError;
			}
			break;
		// Decrement the byte at the data pointer.
		case '-':
			if (-1 == --data[ptr]) {
				errprintf(ip, "Byte overflow (-1).\n");
				++isError;
			}
			break;
		// Output the byte at the data pointer.
		case '.':
			putchar(data[ptr]);
			break;
		// Input a byte and store it at the data pointer.
		case ',':
			data[ptr] = getchar();
			break;
		// If the byte at the data pointer is 0, jump to the corresponding ']'
		case '[': {
			if (0 == data[ptr]) {
				int depth = 1, ipError = ip;;
				while (depth) {
					if (++ip == source_len) {
						errprintf(ipError, "End of loop not found.\n");
						++isError;
						break;
					}

					if (source[ip] == '[') ++depth;
					else if (source[ip] == ']') --depth;
				}
			}
		} break;
		// If the byte at the data pointer is not 0, jump to the
		// corresponding '['
		case ']': {
			if (0 != data[ptr]) {
				int depth = 1, ipError = ip;
				while (depth) {
					if (--ip == -1) {
						errprintf(ipError, "Start of loop not found.\n");
						++isError;
						break;
					}

					if (source[ip] == '[') --depth;
					else if (source[ip] == ']') ++depth;
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

	free(data);
	return !isError;
}

char *read_code(const char *filepath) {
	// Open the file from the path in read mode and handle the error for it.
	FILE *file = fopen(filepath, "r");
	if (NULL == file) {
		fprintf(stderr, "Unable to open file `%s`.\n", filepath);
		return NULL;
	} /* if the file is not open and is NULL */

	fseek(file, 0, SEEK_END);
	int source_len = ftell(file);
	char *source = (char *)malloc(source_len + 1);
	if (NULL == source) {
		fprintf(stderr, "Failed to allocate memory for the source.\n");
		fclose(file);
		return NULL;
	} /* if failed to allocate memory for the source */
	rewind(file);

	for (int i = 0; i < source_len; ++i) source[i] = getc(file);
	source[source_len] = '\0';

	fclose(file);
	return source;
}

void errprintf(int ch, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	fprintf(
		stderr, "\nFile \"%s\", char %d\nError: ",
		bf_compile_filepath, ch + 1
	);
	vfprintf(stderr, fmt, ap);

	va_end(ap);
}