/*
Chris Omlor
Edward Hsu
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

int cur_history_count = 0;
struct Command {
	char pro[10];
        char  command[200];
        int  length;
};

// Stuff for the History functionality
// Command array and stuff.
struct Command *enteredCommands[10] = { NULL };

// Main Program Loop
int my_shell();

// Input Functions
int read_input(char *);
int parseInput(char* input, char*prog, char *args[], int *argc);
int saveCommand(char *prog, char *user_in, int* argc);
int saveToArray(struct Command *in);

// Execute Command
int exArgs(char* prog, char*[], int argc);
int executeCMD( char *prog, char *[], int argc);
static void handler(int sig);
int runHistory(char* prog);
int printHistory();

int main(void)
{
    int exit_code = 0;

    exit_code = my_shell();

    printf("EXIT SUCCESS!\n");
    return 0;
}

int my_shell()
{

	struct sigaction act, act_old;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	if(	(sigaction(SIGINT,  &act, &act_old) == -1) ||
		(sigaction(SIGCHLD,  &act, &act_old) == -1)){ // Ctrl^C
		perror("signal");
		return 1;
	}

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
	printf("\nLeave a space after command for best experience.\n");        
	printf("Shell Command ->> ");
        // Flush the buffer of the output stream
        fflush(stdout);

        // Reformat the memory of the user input to max of 255 characters, clears previous input
        user_in = calloc(255, sizeof(char));

        if(!exit)
            exit = read_input(user_in);

        if(!exit)
            exit = parseInput(user_in, prog, args, &argc);

        printf("Arg Count: %d \n",argc);

        printf("Prog: %s\n",prog);
        for(int i = 0; i < argc; i++)
            printf("Arg[%i] = %s  \n",i,args[i]);

        //printf("size of user_in: %d\n", (int)sizeof(user_in));
        //printf("%s\n",user_in);

        if(strcmp(prog, "exit") == 0){
            exit = 1;
		return exit;
	}
	else if(strcmp(prog, "history") == 0){
		//exit = printHistory();
	}
	else if(prog[0] == '!'){
		//exit = runHistory(prog);
	}
	else{
		//exit = saveCommand(prog, args, argc);
            exit = executeCMD(prog, args, argc);
	}

        /* Clear Variables for next run or exiting */
        prog = malloc(COMMAND_LIMIT * sizeof(char));
        for(int i = 0; i < argc; i++)
            args[i] = malloc(COMMAND_LIMIT * sizeof(char));

        //exit = 0;
    }

    // Free the memory we have used this far
    free(user_in);
    free(prog);
    for(int i = 0; i < argc; i++)
        free(args[i]);

    return exit;
}
/*
//create  command into struct
int saveCommand(char *prog, char *user_in, int* argc){
        struct Command *temp;
	strcpy( temp->pro, prog);
        strcpy( temp->command, user_in);
        temp->length = argc;
        saveToArray(temp);
        return 1;
}

int saveToArray(struct Command *in){
        int i;
        int maxed = 0;

        if(enteredCommands[9]){
                maxed = 1;
                //printf("Array Maxed");
        }

        if(maxed == 0){
                for( i = 0; i < MAX_HISTORY; i = i + 1){
                        if (!enteredCommands[i]){
                                enteredCommands[i] = in;
                                //printf("%d\n", i);
                                break;
                        }
                }
        }
        if(maxed == 1){
                int k = 0;
                //struct Command *newCommandList[10];
                for(k = 0; k < MAX_HISTORY - 1; k = k + 1){
                        enteredCommands[k] = enteredCommands[k + 1];
                }
                enteredCommands[MAX_HISTORY - 1] = in;
                //enteredCommands = newCommandList;
        }
        return 1;
}
int runHistory(char* prog){
	int index = 0;
	if(!enteredCommands[0]){
		printf("No commands in history\n");
		return 0;
	}
	if(prog[1] == '!')
		index = MAX_HISTORY - 1;
	else{
		index = atoi(&prog[1]) - 1;
		if((index < 0) || (index > MAX_HISTORY)){
			fprintf(stderr, "No such command in history.\n");
			return 0;
		}
	}
	printf("Running: %s\n", prog);
	struct Command *temp = enteredCommands[index];
	return executeCMD(temp->pro, temp->command, temp->length);
}
int printHistory(){
        int i;
        for( i = 0; i < MAX_HISTORY; i = i + 1){
                if (enteredCommands[i]){
                        int j = 10 - i;
                        printf("%d %s\n", j, enteredCommands[i]->command);
                }
        }
        return 1;
}*/


// Working...
// Needs Error handling and buffer overflow detection
int read_input(char *user_in)
{
    fgets(user_in,1024,stdin);
    fflush(stdin);
    //printf("size of user_in: %d\n", (int)sizeof(user_in));

    //scanf("%s", user_in);
    //printf("%s\n", user_in);
    return 0;
}

static void handler(int sig){
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

int exArgs(char* prog, char* args[], int argc)
{
    int exit_code = 0;

    if(strcmp(prog, "exit") == 0)
        exit_code = 1;

    return exit_code;
}

int parseInput(char* input, char* prog, char* args[], int *argc)
{
    enum states { SPACE, WORD, LITERAL, EXIT };
    int cur_state = 0;
    int i = 0;
    int dbl_qte = 0;
    char* token;
    int token_index = 0;
    *argc = 0;

    int index;
    index = strcspn(input," ");
    while(i < index)
    {
        token[token_index] = input[i];
        token_index = token_index + 1;
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

