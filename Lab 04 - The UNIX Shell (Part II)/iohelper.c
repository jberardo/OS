/*****************/
/*   includes    */
/*****************/
#include "iohelper.h"
#include "common.h"

/*****************/
/*   functions   */
/*****************/
void printPrompt(void)
{
    printf("minishell > ");
}

/*************************************/
/*  read input from user's keyboard  */
/*************************************/
void read_line(char line[])
{
    fgets(line, LINE_LEN, stdin);
}

/* parse line char array into argv string array */
void parse_arguments(char line[], int *argc, char *argv[])
{
	const char *delimiters = DELIMITERS;
	char *tok;
	*argc = 0;

	tok = strtok(line, delimiters);

	//while((tok = strsep(&line, delimiters)))
	while(tok != NULL)
	{
		/* *argc is the value of the pointer */
		argv[*argc] = tok;
		/* increment number of arguments (pointer value) */
		(*argc)++;

		tok = strtok(NULL, delimiters);
	}

	argv[*argc] = NULL;
}

void getExecutablePath(char *command, char *full_path)
{
	const char *delimiter = ":";
	char *path = getenv("PATH");
	char *tok;

	//char *exec_path = malloc(MAX_PATH_LEN);

	//if (!exec_path)
	//{
	//	return;
	//}

	tok = strtok(path, delimiter);

	while(tok != NULL)
	{
		int total_len;
		char temp[MAX_PATH_LEN];

		total_len = strlen(command) + strlen(tok) + 1;

		if (total_len > MAX_PATH_LEN)
		{
			fprintf(stderr, "executable path length exceeded maximun");
			return;
		}

		strcpy(temp, tok);
		strcat(temp, "/");
		strcat(temp, command);

		if (access(temp, R_OK | X_OK) == 0)
		{
			strcpy(full_path, temp);
			return;
		}

		tok = strtok(NULL, delimiter);
	}
}