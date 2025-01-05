# Getting started
In order to be able to run the program you need to have a C compiler (in our case we use gcc), a virtual machine (if you have non-linux OS), CUnit testing framework, valgrind installed (for checking any memory leaks), and gcov tool installed(for checking the coverage of the tests).

## Building and running program
make test - cleans, compiles and runs tests through valgrind
make full_val_backend_tests - cleans, compiles and runs tests through valgrind with --leak-check=full flag
make start - cleans, compiles and runs frontend 
make clean_full_val_frontend - cleans, compiles and runs frontend through valgrind with --leak-check=full flag

## Coverage tests
The tool used for checking the coverage is gcov. In order to test the coverage of our tests, run the following commands:
make backend_tests_cov //compiles the backend_tests file
make cov_run //runs the compiled file from above
make gcov //takes the coverage and puts it in coverage.txt, where the results can be found

### Current coverage:
Lines executed:91.23%
Branches executed:94.44%


## How to use
Start program with **make start** and then choose the desired operation by typing the corresponding character.


