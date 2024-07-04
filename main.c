#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define DELIM "\r\n\t "
#define RED "\033[0;31m"
#define YELLOW "\x1b[33m"
#define RESET "\e[0m"
/*shell needs
read line done
stor line done 
tokenze line to arges
execute args
free args and line
jmp read line

--> hande error exeed !! cmd var
--> flages impliment !!
--> piping and <> thingy 
*/


char* read_cmd(){
	int buffer_size = 1024;
	int position = 0;
	char *buffer = malloc(sizeof(char) * buffer_size);
	int c;

	if(!buffer){
	fprintf(stderr, "%sshell: Allocation error%s\n", RED, RESET);
    exit(EXIT_FAILURE);
	}

	while(1){
		c = getchar();

		if(c == '\n' && position == 0){
			printf("$ ");
			fflush(stdin);
			continue;		
		}

		if(c == EOF || c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}
		else{
			buffer[position] = c;
		}
		position++;

		if(position >= buffer_size){
			buffer_size += 1024;
			buffer = realloc(buffer,buffer_size);
			
			if(!buffer){
			fprintf(stderr, "%sshell: reAllocation error%s\n", RED, RESET);
			exit(EXIT_FAILURE);	
			}
		}	
	}
}



char **tok_cmd(char *cmd){
	int token_size = 64;
	int pos = 0;

	char **tokens = malloc(token_size * sizeof(char * ));
	char *token;

	if(!tokens){
		fprintf(stderr, "%sshell: Allocation error <tokens>%s\n", RED, RESET);
		exit(EXIT_FAILURE);		
	}

	token = strtok(cmd,DELIM);
	while(token != NULL){
		tokens[pos] = token;	
		pos++;

		if(pos >= token_size){
			token_size += 1024;
			tokens = realloc(tokens,token_size * sizeof(char *));

			if(!tokens){
				fprintf(stderr, "%sshell: reAllocation error <tokens>%s\n", RED, RESET);
				exit(EXIT_FAILURE);			
			}
		}
		token = strtok(NULL,DELIM);
	}
	tokens[pos] = NULL;
	return tokens;
}

int shell_exe(char **tokens){
	pid_t cpid;
	int status;

	if(strcmp(tokens[0],"exit") == 0){	//what about the && command ? 
		return 0;
	}
	if(strcmp(tokens[0],"cd") == 0){
		chdir(tokens[1]);
		return 1;
	}	
	
	cpid = fork();

	if(cpid == 0){	//child process
		if(execvp(tokens[0],tokens) < 0){
			printf("%s: command not found.\n",tokens[0]);
			exit(EXIT_FAILURE);
		}
	}
	else if(cpid < 0){
		printf(RED"error forking"RESET"\n");
	}
	else{
	waitpid(cpid,&status,WUNTRACED);	
	}
	return 1;
}

void shell_loop(){
	char *cmd;
	char **args;
	int status = 1;
	char* dir;
	char* user;
	do
	{
		user = getlogin();
		dir = getcwd(NULL,0);
		printf(YELLOW"%s:",user);
		printf(RED"%s",dir);	
		printf("$ ");	
		cmd = read_cmd();
		args = tok_cmd(cmd);
		status = shell_exe(args);
		free(cmd);
		free(args);
	}while(status == 1);
}

int main(void){
	shell_loop(); 
	return 0;
}
