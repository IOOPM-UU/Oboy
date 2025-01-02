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
ref: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ) -o $@ $(CUNIT_LINK)

# Target for the test executable
ref_test: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ) -o $@ $(CUNIT_LINK)
	valgrind --leak-check=full --show-leak-kinds=all ./ref_test

gdb: ref
	gdb ./ref --tui

cov: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(C_OPTIONS) --coverage $(REF) $(TEST_SRC) $(HASH) $(LIST) -o $@ $(CUNIT_LINK)
	./cov
	
coverage: cov
	gcov -abcfu cov-test.gcda
	gcovr -r . --html --html-details -o coverage.html

# Clean up generated files
clean:
	rm -f src/*.o test/*.o ref ref_test cov coverage.* cov-*