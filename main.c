/*
Chris Omlor
Edward Hsu(301211630)
Jacky Lu
Marcus Hernandez
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#define MAX_HISTORY 10
#define COMMAND_LIMIT 30

struct Command {
	char pro[10];
    char  command[200];
};

// Stuff for the History functionality
// Command array and stuff.
int cur_history_count = 0;
char *my_history[MAX_HISTORY];
struct Command *enteredCommands[MAX_HISTORY] = { NULL };

// Main Program Loop
int my_shell();

// Input Functions
int read_input(char *);
int parseInput(char* input, char*prog, char *args[], int *argc);
int saveCommand(char *user_in);
int saveToArray(struct Command *in);

// Execute Command
int executeCMD( char *prog, char *[], int argc);
//static void handler(int sig);
int runHistory(char* user_input);
void printHistory();

int main(void)
{
    int exit_code = 0;

    for(int i = 0; i < MAX_HISTORY; i++)
        my_history[i] = malloc(255 * sizeof(char));

    exit_code = my_shell();

    //for(int i = 0; i < MAX_HISTORY; i++)
    //    free(my_history[i]);

    printf("EXIT SUCCESS!\n");
    return exit_code;
}

int my_shell()
{
    /*
	struct sigaction act, act_old;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	if(	(sigaction(SIGINT,  &act, &act_old) == -1) ||
		(sigaction(SIGCHLD,  &act, &act_old) == -1)){ // Ctrl^C
		perror("signal");
		return 1;
	}
    */

    int exit = 0;
    char* user_in;
    char* prog;

    prog = malloc(30 * sizeof(char));

    int argc = 0;
    char* args[COMMAND_LIMIT];

    // Format memory for each element of the argument array
    for(int i = 0; i < 10; i++)
        args[i] = malloc(COMMAND_LIMIT * sizeof(char));

    // Main program loop, Runs as long exit is 0, Set to non zero number to quit program
    while(!exit)
    {
        printf("Shell Command ->> ");
        // Flush the buffer of the output stream
        fflush(stdout);

        // Reformat the memory of the user input to max of 255 characters, clears previous input
        user_in = calloc(255, sizeof(char));

        if(!exit)
        {
            exit = read_input(user_in);
        }

        if(!exit)
        {
            if(user_in[0] == '!')
            {
                if(user_in[1] == '!')
                {
                    exit = parseInput(my_history[cur_history_count-1], prog, args, &argc);
                    exit = saveCommand(my_history[cur_history_count-1]);
                }
                else if(isdigit(user_in[1]) != 0)
                {
                    int index = user_in[1] - '0';
                    if(index >= 0 && index < 10)
                    {
                        exit = parseInput(my_history[index], prog, args, &argc);
                        exit = saveCommand(my_history[index]);
                    }
                }
            }
            else
            {
                exit = saveCommand(user_in);
                exit = parseInput(user_in, prog, args, &argc);
            }
        }

        if(!exit)
        {
            if((strcmp(prog, "exit") == 0) || (strcmp(prog, "exit\n") == 0) || (strcmp(prog, "exit\0") == 0))
            {
                exit = 1;
            }
            else if((strcmp(prog, "history") == 0) || (strcmp(prog, "history\n") == 0) || (strcmp(prog, "history\0") == 0))
            {
                printHistory();
            }
            else
            {
                if(argc != 0)
                {
                    exit = executeCMD(prog,args,argc);
                }
            }
        }

        /* Clear Variables for next run or exiting */
        prog = calloc(COMMAND_LIMIT,sizeof(char));
        for(int i = 0; i < argc; i++)
            args[i] = calloc(COMMAND_LIMIT, sizeof(char));
        argc = 0;
    }

    // Free the memory we have used this far
    free(user_in);
    free(prog);
    //for(int i = 0; i < argc; i++)
    //    free(args[i]);

    return exit;
}


//create  command into struct
int saveCommand(char *user_in)
{
    int exit_code = 1;
    int cur_index = 0;

    if(cur_history_count > 9)
    {
        cur_index = cur_history_count % 10;
        my_history[cur_index] = malloc(255 * sizeof(char));
        strcpy(my_history[cur_index], user_in);
        cur_history_count = cur_index + 1;
        exit_code = 0;
    }
    else
    {
        strcpy(my_history[cur_history_count], user_in);
        cur_history_count = cur_history_count + 1;
        exit_code = 0;
    }

    return exit_code;
}

void printHistory()
{
    for(int i = 0; i < cur_history_count; i++)
    {
        printf("%d: %s\n",i,my_history[i]);
    }
}

// Working...
// Needs Error handling and buffer overflow detection
int read_input(char *user_in)
{
    fgets(user_in,1024,stdin);
    fflush(stdin);
    return 0;
}

/*
static void handler(int sig)
{
	switch(sig){
		case SIGTERM:
		case SIGINT:
			break;
		case SIGCHLD:
			//waits for processes to terminate
			while(waitpid(-1, NULL, WNOHANG) > 0);
			break;
	}

}
*/

int parseInput(char* input, char* prog, char* args[], int *argc)
{
    enum states { SPACE, WORD, LITERAL, EXIT };
    int cur_state = 0;
    int i = 0;
    int dbl_qte = 0;
    char* token;
    int token_index = 0;
    *argc = 0;
    token = malloc(30 * sizeof(char));

    int index;
    index = strcspn(input," ");
    while(i < index)
    {
        if(isalnum(input[i]) != 0)
        {
            token[token_index] = input[i];
            token_index = token_index + 1;
        }
        i = i + 1;
    }

    token[token_index] = '\0';
    strcpy(prog, token);
    strcpy(args[*argc], token);
    *argc = *argc + 1;
    i = i + 1;

    token_index = 0;
    token[token_index] = '\0';

    while(input[i] != '\0')
    {
        if(input[i] == '\0')
        {
            cur_state = EXIT;
        }

        switch(cur_state)
        {
        case SPACE:
            if(input[i] == '"')
            {
                cur_state = LITERAL;
                if(dbl_qte == 1)
                {
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                    token[token_index] = '\0';
                    strcpy(args[*argc], token);
                    *argc = *argc + 1;
                    token_index = 0;
                    token[0] = '\0';
                    dbl_qte = 0;
                }
                else
                {
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                    dbl_qte = 1;
                }
            }
            else if(isspace(input[i]) != 0)
            {
                if(dbl_qte == 1)
                {
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                }
            }
            else
            {
                cur_state = WORD;
                token[token_index] = input[i];
                token_index = token_index + 1;
            }

            i = i + 1;
            break;
        case 1:
            if(isspace(input[i]) != 0)
            {
                cur_state = SPACE;
                if(dbl_qte == 1)
                {
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                }
                else
                {
                    token[token_index] = '\0';
                    strcpy(args[*argc], token);
                    *argc = *argc + 1;
                    token_index = 0;
                    token[0] = '\0';
                }
            }
            else if(input[i] == '"')
            {
                cur_state = LITERAL;
                if(dbl_qte == 1)
                {
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                    token[token_index] = '\0';
                    strcpy(args[*argc], token);
                    *argc = *argc + 1;
                    token_index = 0;
                    token[0] = '\0';
                    dbl_qte = 0;
                }
                else
                {
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                    dbl_qte = 1;
                }
            }
            else
            {
                cur_state = WORD;
                token[token_index] = input[i];
                token_index = token_index + 1;
            }
            i = i + 1;
            break;
        case 2:
            if(input[i] == '"')
            {
                cur_state = LITERAL;
                if(dbl_qte == 1)
                {
                    token[token_index] = '\0';
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                    strcpy(args[*argc], token);
                    *argc = *argc + 1;
                    token_index = 0;
                    token[0] = '\0';
                    dbl_qte = 0;
                }
                else
                {
                    dbl_qte = 1;
                }
            }
            else if(isspace(input[i]) != 0)
            {
                cur_state = SPACE;
                if(dbl_qte == 1)
                {
                    token[token_index] = input[i];
                    token_index = token_index + 1;
                }
            }
            else
            {
                cur_state = WORD;
                token[token_index] = input[i];
                token_index = token_index + 1;
            }
            i = i + 1;
            break;
        case 3:
            token[token_index] = input[i];
            token_index = token_index + 1;
            break;
        default:
            i = i + 1;
            break;
        }
    }

    args[*argc] = (char*)0;
    *argc = *argc + 1;

    return 0;
}

int executeCMD(char* prog, char * cmd[], int argc)
{
	//for background child process wait
	//int bg = 0;
	/*if(strcmp(args[argc-1], "&") == 0){
		bg = 1;
		args[--argc] = NULL;
	}*/
	int status;

    pid_t pid;
    pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, ">> ERROR");
        exit(-1); //I think we should exit if it really fails
    }
    else if(pid == 0)
    {
        execvp(cmd[0], cmd);
        perror("execvp");
        exit(0);
    }
    else
    {
	//if(!bg)
        	waitpid(pid, &status, 0);
    }

    return 0;
}

