#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define MAX_HISTORY 10
int cur_history_count = 0;
struct Command {
        char  command[200];
        int  length;
};

// Stuff for the History functionality
// Command array and stuff.
struct Command  *enteredCommands[10] = { NULL };

// Main Program Loop
int my_shell();

// Input Functions
int read_input(char *);
int parseInput(char*);

// Execute Command
int executeCMD(const char*);

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

    while(!exit)
    {
        printf("->> "); // This is our terminal prompt
        fflush(stdout);

        user_in = calloc(255, sizeof(char));

        exit = read_input(user_in);

        //printf("size of user_in: %d\n", (int)sizeof(user_in));
        //printf("%s\n",user_in);

        //History function calls

        //saveCommand(user_in);
        //printHistory();
        //readIn();
        //writeOut();

        exit = executeCMD(user_in);

        exit = 0;
    }

    free(user_in);
    return exit;
}

//create  command into struct
int saveCommand(char *user_in){
        struct Command *temp;
        strcpy( temp->command, user_in);
        temp->length = (int)sizeof(user_in);
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

int printHistory(){
        int i;
        for( i = 0; i < MAX_HISTORY; i = i + 1){
                if (enteredCommands[i]){
                        int j = 10 - i;
                        printf("%d %s\n", j, enteredCommands[i]->command);
                }
        }
        return 1;
}
//write to file
int writeOut(){
        FILE *outFile;

        outFile = fopen("history.txt", "w+");
        int i;
        for( i = 0; i < MAX_HISTORY; i = i + 1){
                if (enteredCommands[i]){
                        fputs(enteredCommands[i]->command, outFile);
                }
        }
        //fputs("Testing stuff", outFile);
        fclose(outFile);
        return 1;
}
//read File needs error checking
int readIn(){
        FILE *inFile;
        inFile = fopen("history.txt", "r");
        char line[255];

        int i = 0;
  		if(inFile){
            while (fgets(line, sizeof(line), inFile)){
                    struct Command *newCommand;
                    newCommand = malloc(sizeof(newCommand));
                    strcpy( newCommand->command, line);
                    newCommand->length = (int)sizeof(line);
                    saveToArray(newCommand);
                    i = i + 1;
                    //printf("%s", line);
            }
        }
		fclose(inFile);
        return 1;
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
    return 1;
}

int executeCMD(const char * cmd)
{
    printf("EXEC FUNCTION\n");
    printf("%s\b", cmd);
    pid_t pid;

    pid = fork();

    if(pid < 0)
        fprintf(stderr, ">> ERROR");
    else if(pid == 0)
        execlp(cmd, cmd, "-la", "/", (char *)NULL);
    else
        wait(NULL);

    return 0;
}
