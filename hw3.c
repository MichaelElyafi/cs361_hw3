#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

void sigint_handler_sigint(int sig){
	char msg[] = "\ncaught sigint\n";
	write(1, msg, sizeof(msg));
	shell_usage();
}

void sigint_handler_sigtstp(int sig){
	char msg[] = "\ncaught sigstp\n";
	write(1, msg, sizeof(msg));
	shell_usage();
}

shell_usage(){
	char line[500];
	while(1){
 		printf("CS361 > ");
//--------------------------------------Line Parsing
		fgets(line, 500, stdin);
		line[strlen(line) - 1] = '\0';
		int i = 0;
		char** argsarray = malloc(1000 * sizeof(char*));
		char* word;

		word = strtok(line, " ");
		while(word) {
			argsarray[i] = word;
			i++;
			word = strtok(NULL, " ");
		}
		argsarray[i] = NULL;
//--------------------------------------Arg Check
	char *IO;
	char *IO_2[100];
	char *IO_Multi[100];
	int flag = 0;
	int temp = 0;
	int pid = 0;
	int status = 0;
	char *fd = '\0';
	i = 0;
	
	while(1){
		if(argsarray[i] == '\0'){
			break;
		}
		if(!strcmp(argsarray[0], "exit")){
			exit(0);
			return 0;
 		}
		if(!strcmp(argsarray[i], ";")){
			flag = 1;
			IO_Multi[0] = argsarray[i + 1];
			if(argsarray[i+2] != '\0'){
				if((!strcmp(argsarray[i+2], ">")) || (!strcmp(argsarray[i+2], "<"))){
					IO_2[0] = argsarray[i+2];
					IO_2[1] = argsarray[i+3];
				}
				else{
					IO_Multi[0] = argsarray[i+1];
					IO_Multi[1] = argsarray[i+2];
				}
			}
			break;
		}
		if(!strcmp(argsarray[i], "<")){
			IO = argsarray[i];
			fd = argsarray[i + 1];
			if(argsarray[i + 2] != '\0'){
				if(!strcmp(argsarray[i + 2], ";")){
					flag = 1;
					IO_Multi[0] = argsarray[i + 3];
					if(argsarray[i+4] != '\0'){
						IO_2[0] = argsarray[i + 4];
						IO_2[1] = argsarray[i + 5];
					}
				}
			}
			break;
		}
		if(!strcmp(argsarray[i], ">")){
			IO = argsarray[i];
			fd = argsarray[i + 1];
			if(argsarray[i + 2] != '\0'){
				if(!strcmp(argsarray[i + 2], ";")){
					flag = 1;
					IO_Multi[0] = argsarray[i + 3];
					if(argsarray[i+4] != '\0'){
						IO_2[0] = argsarray[i + 4];
						IO_2[1] = argsarray[i + 5];
					}
				}
			}
			break;
		}
		i++;
	}
	pid = fork();
	if(pid == 0){
		if(IO != '\0'){
			if(!strcmp(IO, "<")){
				int fd1 = open(fd, O_RDONLY);
				dup2(fd1, 0);
				close(fd1);
			}
			if(!strcmp(IO, ">")){
				int fd1 = open(fd, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH);
				dup2(fd1, 1);
				close(fd1);
			}
		}
		exit(execv(IO_Multi[0], IO_Multi));
	}
	else {
		wait(&status);
		if(flag = 1){
				if(IO_2[0] != '\0'){
					if(!strcmp(IO_2[0], "<")){
						int fd1 = open(fd, O_RDONLY);
						dup2(fd1,0);
						close(fd1);
					}
					if(!strcmp(IO_2[0], ">")){
						int fd1 = open(fd, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH);
						dup2(fd1, 1);
						close(fd1);
					}
				}
				exit(execv(IO_Multi[0], IO_Multi));
			}
		}
		printf("pid:%i status:%i\n", pid, status,  WEXITSTATUS(status));
	}
	}
}

int main(){
	signal(SIGINT, sigint_handler_sigint);
	signal(SIGTSTP, sigint_handler_sigtstp);
	shell_usage();
}