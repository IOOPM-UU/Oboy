# Define compiler and options
C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -Wextra -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit

REF = src/ref.c
test = test/test.c

# Pattern rule to compile .c files into .o files
%.o: %.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

# Target for linking the final executable
ref: src/ref.o
	$(C_COMPILER) $(C_LINK_OPTIONS) src/ref.o -o $@

ref_test: src/ref.o ref.h
	$(C_COMPILER) $(C_LINK_OPTIONS) src/ref.o -o $@ $(CUNIT_LINK)


clean: 
	rm -f *.o ref ref_test