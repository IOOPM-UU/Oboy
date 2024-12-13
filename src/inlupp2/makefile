
clean:
	rm -f *.o frontend backend_tests *.gcno *.gcda *.gcov backend_tests_coverage

frontend: user_interface/frontend.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c business_logic/shop.c business_logic/merch.c business_logic/cart.c business_logic/common.c
	gcc -g -Wall -pedantic user_interface/frontend.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c business_logic/shop.c business_logic/merch.c business_logic/cart.c business_logic/common.c -o frontend

run_frontend:
	./frontend

val_frontend:
	valgrind -s ./frontend

full_val_frontend:
	valgrind -s --leak-check=full ./frontend

clean_frontend:
	make clean; make frontend; make run_frontend

start:
	make clean_frontend

clean_val_frontend:
	make clean; make frontend; make val_frontend

clean_full_val_frontend:
	make clean; make frontend; make full_val_frontend

backend_tests: business_logic/common.c business_logic/shop.c business_logic/merch.c business_logic/cart.c tests/backend_tests.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c
	gcc -g -Wall -pedantic business_logic/common.c business_logic/shop.c business_logic/merch.c business_logic/cart.c tests/backend_tests.c generic_utils/utils.c generic_data_structures/linked_list.c generic_data_structures/hash_table.c -lcunit -o backend_tests

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