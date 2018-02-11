#include "sig.h"

int main(void)
{
    int i, parent_pid, child_pid, status;

    /* prepare sig_handler to catch SIGUSR1 */
    if (signal(SIGUSR1, sig_handler)==SIG_ERR)
    {
        fprintf(stderr, "parent: error creating handler for SIGUSR1\n");
    }

    /* prepare sig_handler to catch SIGUSR2 */
    if (signal(SIGUSR2, sig_handler)==SIG_ERR)
    {
        fprintf(stderr, "parent: error creating handler for SIGUSR2\n");
    }

    if (signal(SIGTERM, sig_term_handler) == SIG_ERR)
    {
        fprintf(stderr, "parent: error creating handler for SIGTERM\n");
    }

    parent_pid = getpid();

    child_pid = fork();

    if (child_pid == 0)
    {
        kill(parent_pid, SIGUSR1);
        for (;;) pause ();
    }
    else
    {
        kill(child_pid, SIGUSR2);
        fprintf(stdout, "parent: terminating child...\n");
        kill(child_pid, SIGTERM);
        wait(&status);
        fprintf(stdout, "parent: done.\n");
    }
}

static void sig_handler(int signo)
{
    switch(signo)
    {
        case SIGUSR1:
            fprintf(stdout, "parent: received SIGUSR1\n");
            break;
        case SIGUSR2:
            fprintf(stdout, "parent: received SIGUSR1\n");
            break;
        default:
            break;
    }

    return;
}

static void sig_term_handler()
{
    fprintf(stdout, "Catching signal term!\n");
    return;
}