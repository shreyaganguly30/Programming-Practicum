# create the executable file to compile demo
build_demo:
	chmod a+x demo.exe
	./demo.exe

# run the executable file for demo
run_demo:
	./demo.exe

# Targets According To The zyBooks Instructions

# a target to compile the code
build_program:
	rm -f main.exe
	gcc main.c -o main.exe

# a target to run the program interactively without testing
run_interactively:
	./main.exe

# target to run my program with testing
run_with_testing:
	./main.exe -t

# a target to run the program interactively under valgrind
valgrind:
	rm -f main.exe
	gcc -g main.c -o main.exe
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./main.exe

# a target to run the program using the redirection operator
# to supply input values non-interactively (you will need to
# make a .txt file that contains preset user-inputs)
redirection_simple3:
	./main.exe < input_simple3.txt

redirection_simple4:
	./main.exe < input_simple4.txt

redirection_simple5:
	./main.exe < input_simple5.txt

# a target to clean executable file
clean:
	rm -f main.exe