#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

// Main Program Loop
int my_shell();

// Input Functions
int read_input(char *);
int parseInput(char*);

// Execute Command
int executeCMD(const char*);
int

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

	int len = strlen(user_in);
	if(line_len == 1){
		continue;
	}
	
        //printf("size of user_in: %d\n", (int)sizeof(user_in));
        //printf("%s\n",user_in);

	if(strcmp(user_in, "exit") == 0){
		break;
	}
	else if(strcmp(line, "history") == 0){
		//use edward's list history
	}
	else if (line[0] == '!'){
		//use edward's run from history
	}
	else{
		//use edward's add to history

        exit = executeCMD(user_in);
	}

        exit = 1; //leaving this for now, but I want to get rid of it
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

	//I want to add '\0' for an eof thing

    //printf("size of user_in: %d\n", (int)sizeof(user_in));

    //scanf("%s", user_in);
    //printf("%s\n", user_in);
    return 1;
}

int executeCMD(const char * cmd)
{
	char * comm = strdup(cmd);
	char *params[10];
	int ind = 0;

	params[ind++] = strtok(comm, " ");
	while(params[ind-1] != NULL){
		params[ind++] = strtok(NULL, " ");
	}

	ind--;
	int dex = 0;
	if(strcmp(params[ind-1], "&") == 0){
		dex = 1;
		params[--ind] = NULL;
	}

	int io[2] = {-1, -1};

	while(ind >= 3){

		if(strcmp(params[ind-2], ">") == 0){
			//LINE 50-77
		}
	}
			

	int stat;
    printf("EXEC FUNCTION\n");
    printf("%s\b", cmd);
    pid_t pid;

    pid = fork();
	//LINE 82+
    if(pid < 0)
        fprintf(stderr, ">> ERROR");
	perror("fork");
	break;
    else if(pid == 0)
        execlp(cmd, cmd, "-la", "/", (char *)NULL);
    else
        wait(NULL);

    return 0;
}

