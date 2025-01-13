clean:
	rm -f *.o frontend backend_tests old_hash old_list *.gcno *.gcda *.gcov backend_tests_coverage

frontend: user_interface/frontend.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c business_logic/shop.c business_logic/merch.c business_logic/cart.c business_logic/common.c
	gcc -g user_interface/frontend.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c business_logic/shop.c business_logic/merch.c business_logic/cart.c business_logic/common.c ../src/refmem.c  ../src/lib/lib_hash_table.c  ../src/lib/lib_linked_list.c -o frontend -w

run_frontend:
	clear; ./frontend

val_frontend:
	valgrind -s ./frontend

full_val_frontend:
	make frontend; clear; valgrind -s --leak-check=full ./frontend

clean_frontend:
	make clean; make frontend; make run_frontend

start:
	clear
	make clean_frontend

clean_val_frontend:
	make clean; make frontend; make val_frontend

clean_full_val_frontend:
	make clean; make frontend; make full_val_frontend

backend_tests: business_logic/common.c business_logic/shop.c business_logic/merch.c business_logic/cart.c tests/backend_tests.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c ../refmem.c
	gcc -g -Wall -pedantic business_logic/common.c business_logic/shop.c business_logic/merch.c business_logic/cart.c tests/backend_tests.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c ../refmem.c  ../lib/lib_hash_table.c  ../lib/lib_linked_list.c -lcunit -o backend_tests

run_backend_tests:
	./backend_tests

val_backend_tests: 
	valgrind -s ./backend_tests

full_val_backend_tests:
	valgrind -s --leak-check=full  ./backend_tests

clean_val_backend_tests:
	make clean; make backend_tests; make val_backend_tests

test:
	clean_val_backend_tests

debug: backend_tests
	gdb ./backend_tests --tui

backend_tests_cov: business_logic/common.c business_logic/shop.c business_logic/merch.c business_logic/cart.c tests/backend_tests.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c
	gcc -g -Wall -pedantic business_logic/common.c business_logic/shop.c business_logic/merch.c business_logic/cart.c tests/backend_tests.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c -lcunit --coverage -o backend_tests_coverage

cov_run:
	./backend_tests_coverage

gcov:
	gcov -b -c backend_tests_coverage-backend_tests.gcda > coverage.txt

old_hash: 
	gcc -g -Wall -pedantic generic_data_structures/hash_table.c generic_data_structures/linked_list.c generic_data_structures/hash_table_tests.c ../refmem.c ../lib/lib_hash_table.c ../lib/lib_linked_list.c -lcunit -o old_hash
	valgrind -s --leak-check=full ./old_hash

old_list:
	gcc -g -Wall -pedantic generic_data_structures/linked_list.c generic_data_structures/linked_list_tests.c ../refmem.c ../lib/lib_hash_table.c ../lib/lib_linked_list.c -lcunit -o old_list
	valgrind -s --leak-check=full ./old_list

old_list_full:
	gcc -g -Wall -pedantic generic_data_structures/linked_list.c generic_data_structures/linked_list_tests.c ../refmem.c ../lib/lib_hash_table.c ../lib/lib_linked_list.c -lcunit -o old_list
	valgrind -s --leak-check=full --show-leak-kinds=all ./old_list

tests_hidden: frontend
	@GREEN='\033[1;32m'; \
	RED='\033[1;31m'; \
	NC='\033[0m'; \
	for file in ./user_interface/ui-tests/*.txt; do \
		base_name=$$(basename $$file .txt); \
		echo "Running test: $$base_name"; \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./frontend < $$file > temp_output.txt 2>&1; \
		if grep -q "ERROR SUMMARY: 0 errors" temp_output.txt; then \
			echo -e "$$base_name $$GREEN PASSED $$NC"; \
		else \
			echo -e "$$base_name $$RED FAILED $$NC"; \
			cat temp_output.txt; \
		fi; \
	done
	@rm -f temp_output.txt

