# Define compiler and options
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -Wextra -g
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit

# Source and object files
REF            = src/ref.c
REF_OBJ        = src/ref.o
TEST_SRC       = test/test.c
TEST_OBJ       = test/test.o

# Pattern rule to compile .c files into .o files
%.o: %.c
	$(C_COMPILER) $(C_OPTIONS) -c $< -o $@

# Target for the reference executable
ref: $(REF_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) -o $@

# Target for the test executable
ref_test: $(REF_OBJ) $(TEST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) -o $@ $(CUNIT_LINK)

# Clean up generated files
clean:
	rm -f src/*.o test/*.o ref ref_test