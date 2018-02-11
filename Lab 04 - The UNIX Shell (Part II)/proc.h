#ifndef PROC_H
#define PROC_H

/*
* includes
*/
#include "common.h"
#include "iohelper.h"

/*
* prototypes
*/
int exec_cmd(char *argv[], int is_background);
int exec_pipe(char* cmd1[], char* cmd2[]);
int exec_redirection(char* argv[], char* target, int direction);

#endif