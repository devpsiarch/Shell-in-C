#include <stdio.h>
#include <string.h>
#define CMD_SIZE 128
#define DELIM "\n\t "
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


void read_cmd(char *dst){
	if(strlen(dst) > CMD_SIZE){
		printf("command exeeded buffer\n");
		return; 
	}	
	fflush(stdin);
	fgets(dst,CMD_SIZE,stdin);    
}

void cmdtok(char *src){
	char *token = strtok(src,DELIM);
	printf("the first token is : %s\n",token);		
	token = strtok(token,DELIM);
	printf("the first token is : %s\n",token);		
	
	 
}

int main(void){
    char cmd[128];
    char **args; 
	
	printf("$ ");
	read_cmd(cmd);   
	//args = cmdtok(cmd); allocate space for the 2D array
	cmdtok(cmd);
	printf("the cmd is : %s\n",cmd);//stops at "\n"
	return 0;
}
