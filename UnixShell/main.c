/* (c) 2022 Hasan Enes Şimşek
 * This code is licensed under MIT license (see LICENSE.txt for details)
 *
 * I've designed a simple Unix Shell while self-studying as described in
 * Operating System Concepts (Silberschatz, Galvin, and Gagne; 2009, 2011, or 2012)
 */



#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_ARGS 20 /* The maximum number of args in command */

/* args elements point the address of words in command string */
/* command will be modified such that every word ends with null char */
void parse_command(char* command, char* args[], int* BackgroundTaskFlag)
{
	int i;
	int wordCounter = 0;
	int isNewWord = 1; /* to indicate next char will be the first char of the next word */
	*BackgroundTaskFlag = 0; /* by default new task is not going to run in bg */
	int len = strlen(command);

	// check each letter
	for(i = 0; i < len; i++)
	{
		switch (command[i])
		{
		// replace whitespaces with null chars
		case ' ':
			command[i] = '\x00';
			isNewWord = 1;
			break;

		// replace newline with null chars
		case '\n':
			command[i] = '\x00';
			break;

		// this char indicates background task 
		case '&':
			*BackgroundTaskFlag = 1;
			break;
		
		default:
			if(isNewWord)
			{
				if(wordCounter >= MAX_ARGS)
				{
					printf("Argument number exceeds the limit\n");
					exit(1);
				}

				args[wordCounter] = &command[i];
				isNewWord = 0;
				wordCounter++;
			}
			break;
		}
	}

	// last element should be NULL for execvp logic
	args[wordCounter] = NULL;
}

int main(void)
{
	char command[MAX_LINE]; /* obtained command */
	char *args[MAX_LINE]; /* parsed args from command */
	int should_run = 1; /* flag to determine when to exit program */
	int BackgroundTaskFlag; /* flag to determine if new program run in bg */
	
	while (should_run) 
	{
		printf("osh>");
		fflush(stdout);

		fgets(command, MAX_LINE, stdin);
		parse_command(command, args, &BackgroundTaskFlag);

		pid_t pid = fork(); /* fork a child process */

		if(pid < 0) 
		{ 
			printf("Fork Failed\n");
			return 1;
		}
		else if(pid == 0)
		{
			int r = execvp(args[0], args);
			if(r < 0)
				printf("child execution result: %d", r);
			return 0;
		}
		else
		{
			if(BackgroundTaskFlag)
			{
				// do not wait child
			}
			else
			{
				wait(NULL); // wait for child to die
			}
		}
	}

	return 0;
}