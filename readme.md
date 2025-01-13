# Getting started
In order to be able to run the program you need to have a C compiler (in our case we use gcc), a virtual machine (if you have non-linux OS), CUnit testing framework, valgrind installed (for checking any memory leaks), and gcov tool installed(for checking the coverage of the tests).


## Building and running program
make - compiles all .o files to the obj directory
make test - cleans, compiles and runs tests through valgrind with --leak-check=full flag
make memtest - cleans, compiles and runs tests through valgrind with --leak-check=full flag
make demo - cleans, compiles and runs frontend 
make memdemo - cleans, compiles and runs frontend 
make clean- cleans all the object files (as well as unnecessary files)