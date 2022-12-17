# Brainfuck
Brainfuck interpreter written in C.

## What is Brainfuck?

Brainfuck is an esoteric programming language created in 1993 by Urban Müller. It is designed to challenge and amuse programmers, and was not made to be suitable for practical use.

Brainfuck uses a simple machine model consisting of an infinite tape of memory cells, each initially set to zero, and a pointer to the current cell. The language has only eight commands, which are each represented by a single character. These commands can be used to manipulate the memory cells and the pointer, and to perform input and output.

Brainfuck is often used as a test of a person's ability to write a language interpreter or a compiler. It is considered a "minimalistic" language, as it has a very small set of commands and is not intended for practical use.

### Commands:

The eight language commands each consist of a single character: 

| Command         | Meaning                                                                 | C equivalent                             |
|:---------------:|-------------------------------------------------------------------------|------------------------------------------|
| (Program Start) |                                                                         | char data[30000] = {0}; char ptr = data; |
| `>`             | Increment the data pointer.                                             | `++ptr;`                                 |
| `<`             | Decrement the data pointer.                                             | `--ptr;`                                 |
| `+`             | Increment the byte at the data pointer.                                 | `++*ptr;`                                |
| `-`             | Decrement the byte at the data pointer.                                 | `--*ptr;`                                |
| `.`             | Output the byte at the data pointer.                                    | `putchar(*ptr);`                         |
| `,`             | Input a byte and store it at the data pointer.                          | `*ptr = getchar();`                      |
| `[`             | If the byte at the data pointer is 0, jump to the corresponding ']'     | `while (*ptr) {`                         |
| `]`             | If the byte at the data pointer is not 0, jump to the corresponding '[' | `}`                                      |

## Usage
	Usage: brainfuck [OPTIONS] [FILE/-e CODE]

	Interpret brainfuck code by passing the file or the code itself.

	Available options:
	  -h, --help          Show this help message.
	  -v, --version       Show the program version.
	  -o, --output FILE   Write output to FILE.
	  -e, --execute CODE  Execute the code specified by CODE.

	For more information or to report bugs, add me on discord sopa#7147

Show help menu without giving any arguments.

## Output:
Prints the result of the program and the time in which it was execute:

	Hello World!
	
	The program took 0.000000 seconds to execute.

### Error handling:
The interpreter has error handling, it prints the path to the file and the character where the error occurred and below the error message.

	File "hello.bf", char 108
	Error: Byte overflow (256).

## Getting the source
Download the source code by running the following code in your command prompt:
```sh
$ git clone https://github.com/jsob0/brainfuck.git
```
or simply [grab](https://github.com/jsob0/brainfuck/archive/main.zip) a copy of the source code as a Zip file.

## Building
Create the build directory.
```sh
$ mkdir build
$ cd build
```
Brainfuck requires CMake and a C compiler (e.g. Clang or GCC) in order to run.
Then, simply create the Makefiles:
```sh
$ cmake ..
```
and finally, build it using the building system you chose (e.g. Make):
```sh
$ make
```
Run the interpreter by specifying the file path or its code.
```sh
$ ./brainfuck -o hello.txt hello.bf
$ ./brainfuck -e "-[------->+<]>-.-[->+++++<]>++.+++++++..+++.[--->+<]>----."
```

## License
The code is released under the MIT License. See [LICENSE](/LICENSE).
