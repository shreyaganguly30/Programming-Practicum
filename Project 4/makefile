# recursively removes the files from app.exe (for clearing old info)
# complies app.c with the functions in MinPopVote.c
# builds the executable app.exe
build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe

# builds the target "run" that executes the testing suite app.exe
run:
	./app.exe

# builds the target "run_quiet" that executes the testing suite app.exe quietly
run_quiet:
	./app.exe -q


valgrind:
	rm -f app.exe
	gcc -g app.c MinPopVote.c -o app.exe
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe -f

# recursively removes the files from test.exe (for clearing old info)
# complies test.c with the functions in MinPopVote.c
# builds the executable test.exe
build_test:
	rm -f test.exe
	gcc test.c MinPopVote.c -o test.exe


# builds the target "run_test" that executes the testing suite test.exe
run_test:
	./test.exe


# builds executable with fast
run_fast:
	./app.exe -f


# builds executable with year 2024
run_current_year:
	./app.exe -y 2024


# builds executable with fast, quiet, and year 2020
run_all_flags:
	./app.exe -f -q -y 2020

# builds extra credit file
build_ext:
	rm -f ext.exe
	gcc ext.c MinPopVote.c -o ext.exe

# runs extra credit file
run_ext:
	./ext.exe -f