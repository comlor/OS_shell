#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

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

        exit = executeCMD(user_in);

        exit = 1;
    }

    free(user_in);
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
