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

/*
* Takes in a command from user
* Puts command and arguments in a char*** array
*/
void readLineAndProcessArgs(char*** execvp_args) {
	printf("Made it here!");
	// Read in a line of input 
	char* input = NULL;
	size_t bufSize = 0;
	int numChars = getline(&input, &bufSize, stdin);
	
	// Tokenize the string based on " " (code from Dr. Yue) 
	char* token = strtok(input, " ");
	
	// Put command in first
	strcpy(execvp_args[0][0], token);
	token = strtok(NULL, " ");
	// Put in its argument
	strcpy(execvp_args[0][1], token);
	execvp_args[0][2] = NULL;
	token = strtok(NULL, " ");
	// Skip the pipe
	token = strtok(NULL, " ");
	strcpy(execvp_args[1][0], token);
	token = strtok(NULL, " ");
	strcpy(execvp_args[1][1], token);
	execvp_args[1][2] = NULL;
	
	printf("Done with process args");
	
	
	// Help from https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
	// loop through tokens
   /*while( token != NULL ) 
   {
      printf( " %s\n", token );
    
      token = strtok(NULL, " ");
   }	*/
}

void main(int argc, char* argv[]){

	char inputs[]="ls -l | wc -c";
	int cmd_cnt = 0;
	int i = 0;

	cmd_cnt = process_args( inputs, cmds, cmd_args);
	
	char*** execvp_args = malloc(2 * sizeof(char**));
	execvp_args[0] = malloc(3 * sizeof(char*));
	execvp_args[1] = malloc(3 * sizeof(char*));
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 3; j++) {
			execvp_args[i][j] = malloc(20 * sizeof(char));
		}
	}
	readLineAndProcessArgs(execvp_args);
	execvp(cmds[1], execvp_args[1]);

	
	/*int fd[2];
	pipe(fd);

	// Piping
	int pid1 = fork();
	if(pid1 == 0) {
		dup2(fd[1], 1);
		close(fd[1]);
		execvp(cmds[0], execvp_args[0]);
	}
	
	int pid2 = fork();
	if(pid2 == 0) {
		dup2(fd[0], 0);
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execvp(cmds[1], execvp_args[1]);
	}
	wait(0);
	wait(0);*/
	
	
	/*
	execvp(cmds[0], execvp_args[0]);
	
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
	free(execvp_args);
	//exec_cmd_pipe(cmds[0], cmd_args[0], fd);
	*/
}

