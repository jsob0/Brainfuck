# Brainfuck
Brainfuck interpreter written in C.

## What is Brainfuck?

Brainfuck is an esoteric programming language created in 1993 by Urban Müller. It is designed to challenge and amuse programmers, and was not made to be suitable for practical use.

Brainfuck uses a simple machine model consisting of an infinite tape of memory cells, each initially set to zero, and a pointer to the current cell. The language has only eight commands, which are each represented by a single character. These commands can be used to manipulate the memory cells and the pointer, and to perform input and output.

Brainfuck is often used as a test of a person's ability to write a language interpreter or a compiler. It is considered a "minimalistic" language, as it has a very small set of commands and is not intended for practical use.

### Commands:

The eight language commands each consist of a single character: 

| Character   | Meaning                                                                 |
|:-----------:|-------------------------------------------------------------------------|
| `>`         | Increment the data pointer.                                             |
| `<`         | Decrement the data pointer.                                             |
| `+`         | Increment the byte at the data pointer.                                 |
| `-`         | Decrement the byte at the data pointer.                                 |
| `.`         | Output the byte at the data pointer.                                    |
| `,`         | Input a byte and store it at the data pointer.                          |
| `[`         | If the byte at the data pointer is 0, jump to the corresponding ']'     |
| `]`         | If the byte at the data pointer is not 0, jump to the corresponding '[' |

## Building:
#### To get started, clone the repository and compile the source code:

Download the source code by running the following code in your command prompt:

	git clone https://github.com/jsob0/brainfuck.git

or simply [grab](https://github.com/jsob0/brainfuck/archive/master.zip) a copy of the source code as a Zip file.
	
	cd brainfuck
	gcc brainfuck.c -o brainfuck

## Usage

	./brainfuck [-vh] <source file>
	-v --version    show interpreter version.
	-h --help       show help menu.
	
Show help menu without giving any arguments.

## Output:
Prints the result of the program and the time in which it was execute:

	Hello World!
	
	The program took 0.001000 seconds to execute.

## Error handling:
The interpreter has error handling:

	File "hello.bf", char 108
	Error: Byte overflow (256).

has support for most commands:

| Character   | Exception                           |
|:-----------:|-------------------------------------|
| `>`         | Pointer index out of range (30001). |
| `<`         | Pointer index out of range (-1).    |
| `+`         | Byte overflow (256).                |
| `-`         | Byte overflow (-1).                 |
| `.`         | -                                   |
| `,`         | -                                   |
| `[`         | End of loop not found.              |
| `]`         | Start of loop not found.            |

## License
The code is released under the MIT License. See [LICENSE](/LICENSE).