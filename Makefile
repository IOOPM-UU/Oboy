# Define compiler and options
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -Wextra -g
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit
C_COVERAGE 	   = gcov -abcfu --coverage
R_COVERAGE	   = gcovr -r . --html --html-details -o coverage.html

# Source and object files
REF            = src/ref.c
REF_OBJ        = src/ref.o
TEST_SRC       = test/test.c
TEST_OBJ       = test/test.o
HASH 		   = src/inlupp2_DONOTTOUCH/generic_data_structures/hash_table.c
HASH_OBJ	   = src/inlupp2_DONOTTOUCH/generic_data_structures/hash_table.o
LIST 		   = src/inlupp2_DONOTTOUCH/generic_data_structures/linked_list.c
LIST_OBJ	   = src/inlupp2_DONOTTOUCH/generic_data_structures/linked_list.o

# Pattern rule to compile .c files into .o files
%.o: %.c
	$(C_COMPILER) $(C_OPTIONS) -c $< -o $@

# Target for the reference executable
ref: $(REF_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) -o $@

# Target for the test executable
ref_test: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ) -o $@ $(CUNIT_LINK)
	valgrind --leak-check=full ./ref_test

gdb: ref_test
	gdb ./ref_test --tui

cov: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)


coverage: cov

	$(C_COVERAGE) $()
	gcov -abcfu cov-webstore.gcda
	gcovr -r . --html --html-details -o coverage.html

# Clean up generated files
clean:
	rm -f src/*.o test/*.o ref ref_test