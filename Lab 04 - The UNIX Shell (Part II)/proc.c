/*****************/
/*   includes    */
/*****************/
#include "proc.h"

/*****************/
/*   functions   */
/*****************/
int exec_cmd(int *argc, char *argv[])
{
    int pid;
    int status;

    pid = fork();

    /* fork error */
    if (pid < 0)
    {
        fprintf(stderr, "Error forking");
        return 1;
    }
    /* child process */
    if (pid == 0)
    {
        //char *path_executable = getExecutablePath(argv[0]);
        char *path_executable = "";
        getExecutablePath(argv[0], path_executable);

        if (path_executable == NULL)
        {
            fprintf(stderr, "Command %s not found\n", argv[0]);
            exit(1);
        }

        if (execve(path_executable, argv, NULL) < 0)
        {
            fprintf(stderr, "Error executing %s\n", path_executable);
            exit(1);
        }
        else
        {

        /* free the allocated path on iohelper.c */
        free(path_executable);
        }
    }
    /* parent process */
    else
    {
        // parent process: wait for child terminate
        wait(&status);

        if (status)
        {
            return 1;
        }
    }

    return 0;
}