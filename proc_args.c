#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>


#define MAX_CMD_CNT  10
#define MAX_CMD_LEN  20
#define MAX_ARG_CNT  10
#define MAX_ARG_LEN  20


char cmds[MAX_CMD_CNT][MAX_CMD_LEN];
char cmd_args[MAX_CMD_CNT][MAX_ARG_CNT][MAX_ARG_LEN];

/*
   input:
       argv holds the input string from user.
       Mulitple  cmds are delimited by the | in the
       input string.
   output:
       prog[ ]   holds each cmd in the input string.
       cmd_args  holds args for each cmd
   return value:
       0:     error
       >=1:   number of cmds in the input

*/
int  process_args( char *argv, char prog[MAX_CMD_CNT][MAX_CMD_LEN], char cmd_args[MAX_CMD_CNT][MAX_ARG_CNT][MAX_ARG_LEN]){

   //Todo
   char delt[ ] = " ";
   int cmd_idx = 0;
   int arg_idx = 0;
   char *tok;
   int  new_cmd = 0;
  
   tok=strtok( argv, delt);
   strcpy(prog[cmd_idx],tok);
   strcpy(cmd_args[cmd_idx][arg_idx], tok);
   arg_idx++;
   while( tok = strtok( NULL,delt) ){
	   // If this token is the pipe, we end the current argument
	   // list with (char)0 and increase the command count
     if( strcmp(tok, "|") == 0 ){
          cmd_args[cmd_idx][arg_idx][0]=(char)0;
          cmd_idx++;
          arg_idx = 0;
          new_cmd = 1; // Flag - is the next thing a new command?
      }else{
		  // If this is a new command, then put it in the 
		  // array of commands
         if( new_cmd == 1 ){
             strcpy(prog[cmd_idx],tok);
			 // We need to also put the command in the argument array
			 // so that execvp works
			 strcpy(cmd_args[cmd_idx][arg_idx], tok);
			 arg_idx++;
             new_cmd = 0;
         }else{
         	strcpy(cmd_args[cmd_idx][arg_idx],tok);
         	arg_idx++;
        }
     } 
   }
  
  cmd_args[cmd_idx][arg_idx][0]=(char)0;
  return cmd_idx;
}

void exec_cmd_pipe(char* cmd, char* args[], int fd[]) {
	// redirect output
	//dup2(fd[1], 1);
	// redirect input
	//dup2(fd[0], 0);
	execvp(cmd, args);
}

void main(int argc, char* argv[]){

	char inputs[]="ls -l | wc -c";
	int cmd_cnt = 0;
	int i = 0;

	cmd_cnt = process_args( inputs, cmds, cmd_args);
	
	// Convert cmd_args to char*** rather than set size 
	char*** args_for_cmds;
	int argCount = 0;
	
	// Allocate memory for args_for_cmds
	int j;
	args_for_cmds = malloc(cmd_cnt * sizeof(char**));
	
	for(i = 0; i < cmd_cnt; i++) {
		args_for_cmds[i] = malloc(10 * sizeof(char*));
		for(j = 0; j < MAX_ARG_CNT; j++) {
			args_for_cmds[i][j] = malloc(20 * sizeof(char));
		}
	}
	
	// Loop through all commands
	for(i = 0; i <= cmd_cnt; i++) {
		printf("%d ", i);
		while(cmd_args[i][argCount][0] != (char)0) {
			printf("%s\n", cmd_args[i][argCount]);
			//strcpy(args_for_cmds[i][argCount], cmd_args[i][argCount]);
			printf("Made it past line %d", argCount);
			argCount++;
		}
		// Add null back in and reset argCount 
		//args_for_cmds[i][argCount][0] = (char)0;
		argCount = 0;
		printf("Made it to the end of the loop\n");
	}

	/*
	int fd[2];
	pipe(fd);

	// Piping
	int pid1 = fork();
	if(pid1 == 0) {
		dup2(fd[1], 1);
		close(fd[1]);
		execvp(cmds[0], args_for_cmds[0]);
	}
	
	int pid2 = fork();
	if(pid2 == 0) {
		dup2(fd[0], 0);
		//dup2(fd[1], 1);
		close(fd[0]);
		//close(fd[1]);
		execvp(cmds[1], args_for_cmds[0]);
	}
	wait(0);
	wait(0);
	*/
	
	
	//test
	// cnt is one less than you think it is - cnt represents the index
	for(i = 0 ; i <= cmd_cnt ; i++ ){
		printf("The %dth cmd is %s   ",i,cmds[i]);
		printf(" args: ");
		for( int j = 0; cmd_args[i][j][0] != 0; j++) {
			printf(" %s ",cmd_args[i][j]);
		}
		printf("\n");
	}
	
	printf("%s\n", cmds[0]);	
	printf("%s\n", cmd_args[0][0]);
	printf("%s\n", cmd_args[0][1]);
	
	char** newargs = malloc(sizeof(char*) * 20);
	for(int i = 0; i < 3; i++) {
		newargs[i] = malloc(sizeof(char) * 10);
	}
	strcpy(newargs[0], "ls");
	strcpy(newargs[1], "-l");
	newargs[2] = NULL;
	execvp(cmds[0], newargs);
	free(newargs);
	free(args_for_cmds);
	//exec_cmd_pipe(cmds[0], cmd_args[0], fd);
}