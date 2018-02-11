#ifndef IOHELPER_H
#define IOHELPER_H

/*
* includes
*/
#include "common.h"

/*
* defines
*/

/*
* global var
*/

/*
* prototypes
*/
void printPrompt(void);
void read_line(char line[]);
/* return: type of execution (single process, pipe, redirection, etc) */
int parse_arguments(char buffer[], int *argc, char *argv[]);
int getExecutablePath(const char *command);
void combine(char* destination, const char* path1, const char* path2);

#endif