# f28hs-2024-25-cwk2-sys

Coursework 2 in F28HS "Hardware-Software Interface" on Systems Programming in C and ARM Assembler

## Contents

This folder contains the following template files for the source code and for the report:
- `master-mind.c` ... the main C program for the implementation, and most aux fcts
- `mm-matches.s`  ... the matching function, implemented in ARM Assembler
- `lcdBinary.c`   ... the low-level code for hardware interaction with LED, button, and LCD;
- `testm.c`       ... a testing function to test C vs Assembler implementations of the matching function
- `test.sh`       ... a script for unit testing the matching function, using the -u option of the main prg

## Building and running the application

You can build the main C program (in `master-mind.c`), and the `testm.c` testing function in linux, by typing
> make all

and run the Master Mind program in debug mode by typing
> make run

and do unit testing on the matching function
> make unit

or alternatively check C vs Assembler version of the matching function
> make test

A test script is available to do unit-testing of the matching function. Run it like this from the command line
> sh ./test.sh

To test whether all tests have been successful you can do
> echo $?

which should print `0`.

## Unit testing

This is an example of doing unit-testing on 2 sequences (C part only):
```
> ./cw2 -u 121 313
0 exact matches
1 approximate matches
```

The general format for the command line is as follows (see template code in `master-mind.c` for processing command line options):
```
./cw2 [-v] [-d] [-s] <secret sequence> [-u <sequence1> <sequence2>]
```

## Wiring

A **green LED**, as output device, should be connected to the RPi2 using **GPIO pin 26.**

A **red LED**, as output device, should be connected to the RPi2 using **GPIO pin 5.**

A **Button**, as input device, should be connected to the RPi2 using **GPIO pin 19.**

An **LCD display**, with a potentiometer to control contrast, should be wired to the
Raspberry by as shown in the Fritzing diagram below.

You will need resistors to control the current to the LED and from the Button. You
will also need a potentiometer to control the contrast of the LCD display.

The Fritzing diagram below visualises this wiring. 

![Fritzing Diagram](fritz_CW2_2025_bb.png "Fritzing Diagram with LED and Button")
