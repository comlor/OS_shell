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

	if(exit == -1){
		if
	}

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
			io[1] = open(params[argc-1], 0_CREATE|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|SIRGRP|S_IWGRP);
			if(io[1] == -1){
				perror("open");
				free(comm);
				return;
			}

			params[argc-2] = NULL;
			argc -= 2;
		}else if (strcmp(params[argc-2], "<") == 0) {
			io[0] = open(params[argc-1], O_RDONLY);
			if (io[0] == -1){
				perror("open");
				free(comm);
				return;
			}
			params[argc-2] = NULL;
			argc -= 2;
		} else {
			break;
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
    else if(pid == 0){
        /*execlp(cmd, cmd, "-la", "/", (char *)NULL);*/
	if (io[0] != -1) {
		if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
			perror("dup2");
			exit(1);
		}
	}
	if (io[1] != -1){
		if(dup2(io[1], STDOUT_FILENO) != STDOUT_FILENO){
			perror("dup2");
			exit(1);
		}
	}
	execvp(params[0], params);
	perror("execvp");
	exit(0);
    }else {
	close(io[0]);
	close(io[1]);
	
	if (!background)
		waitpid(pid, &status, 0);
	break;
        /*wait(NULL);*/

    return 0;
}

