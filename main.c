#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define COMMAND_LIMIT 30
// Main Program Loop
int my_shell();

// Input Functions
int read_input(char *);
int parseInput(char* input, char*prog, char *args[], int *argc);

// Execute Command
int exArgs(char* prog, char*[], int argc);
int executeCMD(const char *prog, const char *[], const int argc);


int main(void)
{
    int exit_code = 0;

    exit_code = my_shell();

    printf("EXIT SUCCESS!\n");
    return 0;
}

int my_shell()
{
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
        printf("->> ");
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

        //if(!exit)
        //    exit = exArgs(prog, args, argc);

        if(!exit)
            exit = executeCMD(prog, args, &argc);

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

int exArgs(char* prog, char* args[], int argc)
{
    int exit_code = 0;

    if(strcmp(prog, 'exit') == 0)
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

int executeCMD(const char* prog, const char * cmd[], const int argc)
{
    pid_t pid;
    pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, ">> ERROR");
    }
    else if(pid == 0)
    {
        execvp(prog, cmd);
    }
    else
    {
        wait(NULL);
    }

    return 0;
}
