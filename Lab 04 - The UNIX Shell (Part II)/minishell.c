/*
* includes
*/
#include "minishell.h"

/*
* main - program entry
*/
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

/*
* main program loop
* return 0 on success or non-0 on error
*/
int run(void)
{
	int argc;
	char *argv[MAX_ARG_LEN + 1];
	char buffer[LINE_LEN + 1];
	int is_background = 0;
	int pipe_position, n = 0;

	while(1)
	{
		/* print prompt */
		printPrompt();

		/* read user command from stdin */
		read_line(buffer);

		/* parse arguments in buffer into argv */
		/* argc will be the number of arguments (and index for arrays) */
		parse_arguments(buffer, &argc, argv);

		/* no args */
		if(argc == 0 || strcmp(argv[0], "") == 0)
		{
			continue;
		}

		 /* if exit or quit, terminate */
		if((strcmp(argv[0], "exit") == 0) || (strcmp(argv[0], "quit") == 0))
		{
			return(EXIT_SUCCESS);
		}

		/* search for metacharacters */
		for (n = 0; n < MAX_ARG_LEN; n++)
		{
			/* & - run command in backgorund */
			if (argv[0][n] == '&')
			{
				is_background = 1;
			}
			/* | - run piped commands */
			if (argv[0][n] ==  '|')
			{
				if (pipe_position == 0)
				{
					pipe_position = n;
				}
			}
			/* < - redirect output */
			if (argv[0][n] == '<')
			{
				printf("< found\n");
			}
			/* > - redirect input */
			if (argv[0][n] == '>')
			{
				printf("> found\n");
			}
		}

		/* run commands with pipe */
		if (pipe_position > 0)
		{
			exec_pipe(argv, argv);
		}
		/* run command in foreground or background */
		else
		{
			exec_cmd(argv, is_background);
		}

		/* reset some variables */
		pipe_position = 0;
		n = 0;
		is_background = 0;
	}

	return EXIT_FAILURE;
}
