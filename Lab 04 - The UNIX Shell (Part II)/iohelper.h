/*****************/
/*   includes    */
/*****************/
#include "common.h"

/*****************/
/*   defines     */
/*****************/

/*****************/
/*   global var  */
/*****************/

/*****************/
/*   prototypes  */
/*****************/
void printPrompt(void);
void read_line(char line[]);
void parse_arguments(char buffer[], int *argc, char *argv[]);
char* getExecutablePath(char *command);