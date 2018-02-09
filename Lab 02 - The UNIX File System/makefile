#
# Default rule to execute when the make command has no arguments
#
all: ksamp

#
# preprocessor + compiler + assembler step (produces object files) 
#
ksamp.o: ksamp.c ksamp.h
	gcc -c ksamp.c -o ksamp.o -Wall -pedantic

#
# linker step (produces final executable file)
#
ksamp: ksamp.o
	gcc ksamp.o -o ksamp -Wall -pedantic

#
# The command line 'make clean' executes the follwing command
# (removes all files created during build)
#
clean:
	rm -f ksamp ksamp.o

#
# Lists the "phony" rules in this file ('all' and 'clean')
#
.PHONY: all clean
