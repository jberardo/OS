CC = gcc
FLAGS = -g -Wall -pedantic
OPT_FLAGS = -std=c99
TARGET = minishell
OBJ_FILES = minishell.o iohelper.o proc.o

#
# Default rule to execute when the make command has no arguments
#
all: $(TARGET)

#
# preprocessor + compiler + assembler step (produces object files) 
#
proc.o: proc.c proc.h common.h iohelper.h
	$(CC) $(FLAGS) $(OPT_FLAGS) -c proc.c -o proc.o

iohelper.o: iohelper.c iohelper.h common.h
	$(CC) $(FLAGS) $(OPT_FLAGS) -c iohelper.c -o iohelper.o

minishell.o: minishell.c minishell.h common.h iohelper.h proc.h
	$(CC) $(FLAGS) $(OPT_FLAGS) -c minishell.c -o minishell.o

#
# linker step (produces final executable file)
#
$(TARGET): $(OBJ_FILES)
	$(CC) $(FLAGS) $(OPT_FLAGS) $(OBJ_FILES) -o $(TARGET)

#
# The command line 'make clean' executes the follwing command
# (removes all files created during build)
#
clean:
	rm -f $(OBJ_FILES) $(TARGET) *~

#
# Lists the "phony" rules in this file ('all' and 'clean')
#
.PHONY: all clean
