// C functions to look at the man page:
// 		- getenv
//		- access
//		- fork
//		- execve
//		- wait
//		- fprintf (other than stdout)
//		- gets

// Steps:
// 		- print prompt to stdout
//		- read command from stdin (block until <enter>)
//		- parse into argv
//		- if "exit || quit" break loop and terminate
//		- if not: search for executable file
//		- create child: fork
//		- wait child terminate
//		- child: execve
//		- when child terminates, parent get control back and starts over

/*****************/
/*   includes    */
/*****************/
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/signal.h>
#include <sys/wait.h>

/*****************/
/*   defines     */
/*****************/
#define LINE_LEN		80
#define MAX_ARGS		64
#define MAX_ARG_LEN		64
#define MAX_PATHS		64
#define MAX_PATH_LEN	96
#define WHITESPACE		" .,\t&"
#define DELIMITERS		" \t\r\n\a"

/*****************/
/*   global var  */
/*****************/


/*****************/
/*   prototypes  */
/*****************/
