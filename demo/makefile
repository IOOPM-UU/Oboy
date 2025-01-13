# Define compiler and options
CC              = gcc
CFLAGS          = -g -Wall -pedantic -Wextra -w
LDFLAGS         = -lcunit -lm
OBJ_DIR         = ../obj
SRC_DIR         = ../src
GEN_UTILS_DIR   = generic_utils
GEN_DS_DIR      = generic_data_structures
BUS_LOGIC_DIR   = business_logic
TESTS_DIR       = tests
UI_DIR          = user_interface
LIB_DIR         = ../src/lib



# Source files for frontend
FRONTEND_SRC = $(UI_DIR)/frontend.c $(GEN_UTILS_DIR)/utils.c $(GEN_DS_DIR)/linked_list.c \
               $(GEN_DS_DIR)/hash_table.c $(BUS_LOGIC_DIR)/shop.c $(BUS_LOGIC_DIR)/merch.c \
               $(BUS_LOGIC_DIR)/cart.c $(BUS_LOGIC_DIR)/common.c $(SRC_DIR)/refmem.c \
               $(LIB_DIR)/lib_hash_table.c $(LIB_DIR)/lib_linked_list.c

# Source files for backend tests
BACKEND_TEST_SRC = $(BUS_LOGIC_DIR)/common.c $(BUS_LOGIC_DIR)/shop.c $(BUS_LOGIC_DIR)/merch.c \
                   $(BUS_LOGIC_DIR)/cart.c $(TESTS_DIR)/backend_tests.c $(GEN_UTILS_DIR)/utils.c \
                   $(GEN_DS_DIR)/linked_list.c $(GEN_DS_DIR)/hash_table.c $(SRC_DIR)/refmem.c \
                   $(LIB_DIR)/lib_hash_table.c $(LIB_DIR)/lib_linked_list.c

# Targets
.PHONY: all clean run valgrind debug test

all: frontend backend_tests

# Frontend target
frontend: $(FRONTEND_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $^ -o $(OBJ_DIR)/frontend $(LDFLAGS)

run_frontend: frontend
	clear
	./$(OBJ_DIR)/frontend

val_frontend: frontend
	valgrind -s ./$(OBJ_DIR)/frontend

full_val_frontend: frontend
	clear
	valgrind -s --leak-check=full ./$(OBJ_DIR)/frontend

clean_frontend:
	make clean; make frontend; make run_frontend

# Backend tests target
backend_tests: $(BACKEND_TEST_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $^ -o $(OBJ_DIR)/backend_tests $(LDFLAGS)

run_backend_tests: backend_tests
	./$(OBJ_DIR)/backend_tests

val_backend_tests: backend_tests
	valgrind -s ./$(OBJ_DIR)/backend_tests

full_val_backend_tests: backend_tests
	valgrind -s --leak-check=full ./$(OBJ_DIR)/backend_tests

clean_val_backend_tests:
	make clean; make backend_tests; make val_backend_tests

test: clean_val_backend_tests

# Debug target
debug: backend_tests
	gdb ./$(OBJ_DIR)/backend_tests --tui

# Coverage target
backend_tests_cov: $(BACKEND_TEST_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $^ -o $(OBJ_DIR)/backend_tests_coverage --coverage $(LDFLAGS)

cov_run: backend_tests_cov
	./$(OBJ_DIR)/backend_tests_coverage

gcov:
	gcov -b -c $(OBJ_DIR)/backend_tests_coverage.gcda > coverage.txt

# Old hash target
old_hash: $(GEN_DS_DIR)/hash_table.c $(GEN_DS_DIR)/linked_list.c $(GEN_DS_DIR)/hash_table_tests.c $(SRC_DIR)/refmem.c $(LIB_DIR)/lib_hash_table.c $(LIB_DIR)/lib_linked_list.c
	$(CC) $(CFLAGS) $^ -o $(OBJ_DIR)/old_hash $(LDFLAGS)
	valgrind -s --leak-check=full ./$(OBJ_DIR)/old_hash

# Old list target
old_list: $(GEN_DS_DIR)/linked_list.c $(GEN_DS_DIR)/linked_list_tests.c $(SRC_DIR)/refmem.c $(LIB_DIR)/lib_hash_table.c $(LIB_DIR)/lib_linked_list.c
	$(CC) $(CFLAGS) $^ -o $(OBJ_DIR)/old_list $(LDFLAGS)
	valgrind -s --leak-check=full ./$(OBJ_DIR)/old_list

old_list_full: old_list
	valgrind -s --leak-check=full --show-leak-kinds=all ./$(OBJ_DIR)/old_list

# Hidden tests
tests_hidden: frontend
	@GREEN='\033[1;32m'; \
	RED='\033[1;31m'; \
	NC='\033[0m'; \
	for file in ./$(UI_DIR)/ui-tests/*.txt; do \
		base_name=$$(basename $$file .txt); \
		echo "Running test: $$base_name"; \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(OBJ_DIR)/frontend < $$file > temp_output.txt 2>&1; \
		if grep -q "ERROR SUMMARY: 0 errors" temp_output.txt; then \
			echo -e "$$base_name $$GREEN PASSED $$NC"; \
		else \
			echo -e "$$base_name $$RED FAILED $$NC"; \
			cat temp_output.txt; \
		fi; \
	done
	@rm -f temp_output.txt

# Clean target
clean:
	rm -rf $(OBJ_DIR) *.gcno *.gcda *.gcov coverage.txt
	rm -f frontend backend_tests old_hash ol
