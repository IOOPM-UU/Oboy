# Define compiler and options
C_COMPILER     = gcc
C_OPTIONS      = 
C_LINK_OPTIONS = -lm
CUNIT_LINK     = -lcunit
C_COVERAGE 	   = gcov -abcfu --coverage
R_COVERAGE	   = gcovr -r . --html --html-details -o coverage.html

# Source and object files
REF            = src/refmem.c
TEST_SRC       = test/test.c
HASH 		   = src/lib/lib_hash_table.c
LIST 		   = src/lib/lib_linked_list.c


# Object files
OBJ_DIR        = obj
SRC_DIR		   = src
LIB_DIR		   = lib
TEST_DIR	   = test
REF_OBJ        = $(OBJ_DIR)/refmem.o
TEST_OBJ       = $(OBJ_DIR)/$(TEST_DIR)/test.o
HASH_OBJ       = $(OBJ_DIR)/$(LIB_DIR)/lib_hash_table.o
LIST_OBJ       = $(OBJ_DIR)/$(LIB_DIR)/lib_linked_list.o

CREATE_DIRS = mkdir -p $(OBJ_DIR) $(OBJ_DIR)/$(LIB_DIR) $(OBJ_DIR)/$(TEST_DIR)

# Pattern rule to compile .c files into .o files
$(OBJ_DIR)/%.o: src/%.c 
	$(C_COMPILER) $(C_OPTIONS) -c $< -o $@

$(OBJ_DIR)/lib/%.o: src/lib/%.c 
	$(C_COMPILER) $(C_OPTIONS) -c $< -o $@

$(OBJ_DIR)/test/%.o: test/%.c 
	$(C_COMPILER) $(C_OPTIONS) -c $< -o $@

# Target for the reference executable
all: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	CREATE_DIRS
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ) -o  $@ $(CUNIT_LINK)

run: refmem
	valgrind --leak-check=full --show-leak-kinds=all ./refmem

demo: 
	$(MAKE) -C demo run_frontend

memdemo:
	$(MAKE) -C demo full_val_frontend

# Target for the test executable
test: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ) -o $(OBJ_DIR)/test/test $(CUNIT_LINK)
	$(OBJ_DIR)/test/test
	$(MAKE) -C demo backend_tests
	$(MAKE) -C demo run_backend_tests

memtest: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ) -o $(OBJ_DIR)/test/test $(CUNIT_LINK)
	valgrind --leak-check=full --show-leak-kinds=all $(OBJ_DIR)/test/test
	$(MAKE) -C demo backend_tests
	$(MAKE) -C demo full_val_backend_tests


ref:  $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) -g $(C_LINK_OPTIONS) $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ) -o $@ $(CUNIT_LINK)

gdb: ref
	gdb ./ref --tui

cov: $(REF_OBJ) $(TEST_OBJ) $(HASH_OBJ) $(LIST_OBJ)
	$(C_COMPILER) $(C_LINK_OPTIONS) $(C_OPTIONS) --coverage $(REF) $(TEST_SRC) $(HASH) $(LIST) -o $@ $(CUNIT_LINK)
	./cov
	
coverage: cov
	gcov -abcfu cov-ref.gcda
	gcovr -r . --html --html-details -o coverage.html

# Clean up generated files
clean:
	rm -f obj/*.o obj/*/*.o obj/test/test obj/backend_tests obj/frontend *.gcda *.gcno ref* cov* test.c.*

.PHONY: demo all