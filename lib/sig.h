/*
*   INCLUDES
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

/*
*   PROTOTYPES
*/
int main(void);
static void sig_handler(int signo);
static void sig_term_handler();