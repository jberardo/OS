/*
* includes
*/
#include "iohelper.h"
#include "common.h"

/*
* functions
*/
void printPrompt(void)
{
    printf("minishell > ");
}

/*
* read input from user's keyboard
*/
void read_line(char line[])
{
    fgets(line, LINE_LEN, stdin);
}

/*
* parse line char array into argv string array
*/
int parse_arguments(char line[], int *argc, char *argv[])
{
	/* local variables */
	const char *delimiters = DELIMITERS;
	char *tok;
	*argc = 0;

	/* get first token (before loop) */
	tok = strtok(line, delimiters);

	while(tok != NULL)
	{
		/* '*argc' is the value of the pointer */
		argv[*argc] = tok;

		/* increment number of arguments (pointer value) */
		(*argc)++;

		/* get next token */
		tok = strtok(NULL, delimiters);
	}

	argv[*argc] = NULL;

	return EXIT_SUCCESS;
}

/*
* try to find an executable in PATH env variable
*/
int getExecutablePath(const char *command)
{
	const char *delimiter = ":";
	char *path = getenv("PATH");
	char *tok;

	if (path == NULL)
	{
		fprintf(stderr, "PATH variable not found\n");
		exit(EXIT_FAILURE);
	}

	tok = strtok(path, delimiter);

	while(tok != NULL)
	{
		int total_len = 0;

		total_len = strlen(command) + strlen(tok) + 1;

		if (total_len > MAX_PATH_LEN)
		{
			fprintf(stderr, "executable path length exceeded maximun");
			return 1;
		}

		char full_path[strlen(command) + strlen(tok) + 1];
		/* create full path string */
		combine(full_path, tok, command);

		/* check if file exists and is executable */
		if (access(full_path, R_OK | X_OK) == 0)
		{
			return 0;
		}

		/* get next token */
		tok = strtok(NULL, delimiter);
	}

	/* not found */
	return 1;
}

void combine(char* dst, const char* str1, const char* str2)
{
    if(str1 == NULL && str2 == NULL)
	{
        strcpy(dst, "");
		return;
    }

    if(str2 == NULL || strlen(str2) == 0)
	{
        strcpy(dst, str1);
		return;
    }

    if(str1 == NULL || strlen(str1) == 0)
	{
        strcpy(dst, str2);
		return;
    } 

	char separator[] = "/";
	const char *last_char = str1;

	while(*last_char != '\0')
	{
		last_char++;
	}

	int append_separator = 0;

	if(strcmp(last_char, separator) != 0)
	{
		append_separator = 1;
	}

	strcpy(dst, str1);
	
	if(append_separator)
	{
		strcat(dst, separator);
	}
	
	strcat(dst, str2);

	return;
}