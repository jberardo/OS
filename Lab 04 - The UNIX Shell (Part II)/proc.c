/*
* includes
*/
#include "proc.h"

/*
* execute a single command
* is_background: run process in background or foreground
*/
int exec_cmd(char *argv[], int is_background)
{
    int pid;
    int status;

    pid = fork();

    /* fork error */
    if (pid < 0)
    {
        fprintf(stderr, "Error forking process");
        return EXIT_FAILURE;
    }
    /* child process */
    else if (pid == 0)
    {
        /* try to find executable in PATH */
        if (getExecutablePath(argv[0]) != 0)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        
        if (strcmp(full_path, "") == 0)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(full_path, argv, NULL) < 0)
        {
            fprintf(stderr, "%s: error executing command\n", full_path);
            exit(EXIT_FAILURE);
        }
    }
    /* parent process */
    else
    {
        /* run process in background */
        if (is_background == 1)
        {
            fprintf(stdout, "PID %d running in background...\n", pid);
        }
        /* wait for child terminate */
        else
        {
            wait(&status);

            if (status)
            {
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

/*
* Execute redirection metacharacters (">", and "<")
* argv[]: command name and arguments passed
* target: file to redirect the in/out
* direction: 0 - stdin
*            1 - stdout
* ex: man socket > file1.txt file2.txt file3.txt
*   should redirect the output of:
*       - argv[0]: man
*       - argv[1]: socket
*       - target: file1.txt
*       - direction: 1 (output of man to files)
*/
int exec_redirection(char* argv[], char* target, int direction)
{
    int pid;
    int status;

    pid = fork();

    /* fork error */
    if (pid < 0)
    {
        fprintf(stderr, "Error forking process");
        return EXIT_FAILURE;
    }
    /* child process */
    else if (pid == 0)
    {
        /* replace stdin */
        if (direction == 0)
        {
            int fid = open(target, O_WRONLY | O_CREAT);
            close(STDIN_FILENO);
            dup(fid);
            close(fid);
        }
        /* replace stdout */
        else if (direction == 1)
        {
            int fid = open(target, O_WRONLY | O_CREAT);
            close(STDOUT_FILENO);
            dup(fid);
            close(fid);
        }

        if (getExecutablePath(argv[0]) != 0)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        
        if (strcmp(full_path, "") == 0)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(full_path, argv, NULL) < 0)
        {
            fprintf(stderr, "%s: error executing command\n", full_path);
            exit(EXIT_FAILURE);
        }
    }
    /* parent process */
    else
    {
        /* parent process: wait for child terminate */
        wait(&status);

        if (status)
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

/*
* Execute pipes
*/
int exec_pipe(char* cmd1[], char* cmd2[])
{
    int pid;
    int status;
    int fds[2];

    if (pipe(fds) == -1)
    {
        fprintf(stderr, "Error during pipe\n");
        return EXIT_FAILURE;
    }

    pid = fork();

    /* fork error */
    if (pid < 0)
    {
        fprintf(stderr, "Error forking process\n");
        return EXIT_FAILURE;
    }
    /* child process */
    else if (pid == 0)
    {
        /* close child stdout */
        close(STDOUT_FILENO);
        /* to replace with pipe stdout */
        dup(fds[1]);
        
        /* close pipe */
        close(fds[0]);
        close(fds[1]);

        if (getExecutablePath(cmd1[0]) != 0)
        {
            fprintf(stderr, "%s: command not found\n", cmd1[0]);
            exit(EXIT_FAILURE);
        }
        
        if (strcmp(full_path, "") == 0)
        {
            fprintf(stderr, "%s: command not found\n", cmd1[0]);
            exit(EXIT_FAILURE);
        }

        execve(cmd1[0], cmd1, NULL);
        fprintf(stderr, "error executing child\n");
        exit(EXIT_FAILURE);
    }
    /* parent process */
    else
    {
        /* close parent stdin */
        close(STDIN_FILENO);
        /* to replace with pipe stdout */
        dup(fds[0]);

        /* close pipe */
        close(fds[0]);
        close(fds[1]);

        if (getExecutablePath(cmd2[0]) != 0)
        {
            fprintf(stderr, "%s: command not found\n", cmd2[0]);
            exit(EXIT_FAILURE);
        }
        
        if (strcmp(full_path, "") == 0)
        {
            fprintf(stderr, "%s: command not found\n", cmd2[0]);
            exit(EXIT_FAILURE);
        }
        
        if(execve(cmd1[2], cmd2, NULL) < 0)
        {
            fprintf(stderr, "error executing parent\n");
            return EXIT_FAILURE;
        }

        /* wait for child terminate */
        wait(&status);

        if (status)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}