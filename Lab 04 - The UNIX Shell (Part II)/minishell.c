/*****************/
/*   includes    */
/*****************/
#include "minishell.h"

/*****************/
/*     main      */
/*****************/
int main (void)
{
	/* return status from main loop */
	int status;

	/* run main loop */
	status = run();

	/* return 0 on success or 1 on error */
	if (status)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/* main program loop */
/* return 0 on success or non-0 on error */
int run(void)
{
	int argc;
	char *argv[MAX_ARG_LEN];
	char buffer[LINE_LEN];

	while(1)
	{
		// print prompt
		printPrompt();

		// read user command from stdin
		//fgets(buffer, sizeof(buffer), stdin);
		read_line(buffer);

		parse_arguments(buffer, &argc, argv);

		// no args
		if(argc == 0 || strcmp(argv[0], "") == 0)
		{
			continue;
		}

		 // if exit or quit, terminate
		if((strcmp(argv[0], "exit") == 0) || (strcmp(argv[0], "quit") == 0))
		{
			exit(0);
		}

		exec_cmd(&argc, argv);
	}
}
