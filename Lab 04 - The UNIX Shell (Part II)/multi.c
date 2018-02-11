#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int create_pipe(char cmd1[], char cmd2[], char *args[]);

int main(void)
{
    int n;

    char cmd1[80] = "ls";
    char cmd2[80] = "wc";
    char *args[80];

    strcpy(args[0], "");
    strcpy(args[1], "-l");

    n = create_pipe(cmd1, cmd2, args);

    return 0;
}

int create_pipe(char cmd1[], char cmd2[], char *args[])
{
    int pid = 0;
    int status = 0;

    int fd[2];
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed\n");
        exit(1);
    }

    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Error forking process\n");
        exit(1);
    }
    if (pid == 0)
    {
        /* close stdout */
        close(0);
        /* replace stdout with pipe write end */
        dup(fd[0]);
        /* close fd */
        close(fd[0]);
        close(fd[1]);

        execve(cmd1, args, NULL);
        fprintf(stderr, "Error executing %s\n", cmd1);
        exit(1);
    }
    else
    {
        /* close stdin */
        close(1);
        /* replace stdin with pipe read end */
        dup(fd[1]);
        /* close fd */
        close(fd[0]);
        close(fd[1]);

        execve(cmd2, args, NULL);
        fprintf(stderr, "Error executing %s\n", cmd2);
        exit(1);        
    }

    return 0;
}