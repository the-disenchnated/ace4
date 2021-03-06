#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#define MAX 512
#define pointer ">"

char *par[MAX];
char input[1000];
const char* home;
char history[20][MAX];
int historyCount=0;
int histLoop = 0;


char *inter[] = {"exit", "getpath", "setpath", "cd","history"};

void exit(int status); /*function for exiting the program*/

void printHistory(){
	int i;
	int j;
	if(histLoop == 0){
		for(i = 0; i< historyCount;i++){
			printf("%i %s",i+1,history[i]);
		}
	}
	else{
	for(j = 0; j< 20;j++){
			printf("%i %s",j+1,history[j]);
		}
	}


	
}



void saveHistory(){

	int i;
	for(i = 19;i >=0; i-- ){
		strcpy(history[i+1],history[i]);
	}

	strcpy(history[0],input);
	historyCount++;

	if(historyCount >19){
		histLoop = 1;
		historyCount = 0;
	}
}


void executeCMD(){

	int status;
	pid_t pid;
	/* fork a child process */
	pid = fork();
	
	if (pid == 0) { /* child process */
		if (execvp(par[0], par)){
			printf("%s is an Unknown Command 1\n", *par);
		exit(0);
		}
	}
	else if (pid < 0) { /* error occurred */
		printf("Fork Failed");
		exit(1);
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		while(wait(&status)!=pid);
	}	
}


void runInternal(int m){
int result;
char *promptBuff;
promptBuff = NULL;
	switch(m){
		case 0:
			home = getenv("HOME");
			int i = chdir(home);
			if(i<0) printf("Couldn't restore\n");
			else{
				printf("directory changed\n");
				printf("HOME: %s\n", home);
			}
			exit(0);
		case 1:
			/*if(par[1] != NULL){
					printf("%s is an Unknown command 2\n", *par);
			}
			else{*/
				printf("PATH = %s\n", getenv("PATH"));
				/*}*/
			break;
		case 2:
			if(par[1] == NULL){
					printf("%s is an Unknown command 3\n", *par);
			}
			else{
				int newPath = setenv(par[1], par[1], 0);
				printf("%d", newPath);
				/*const char* path = getenv("PATH");
				printf("PATH :%s\n",(path!=NULL)? path : "getenv returned NULL");*/
			}
			break;
		case 3:
			
			result= 0;
			if(par[1] == NULL){
				chdir("..");
			}
			else{
				result = chdir(par[1]);
				if (getcwd(promptBuff, sizeof(par[1])) != NULL);
				else{
					printf("FAILED\n");
				}
				if(result == 0){
					printf("Directory changed to %s\n", par[1]);
				}	
				else{
					switch(result){
					case EACCES: perror("Permission denied");
						break;
					case EIO:	 perror("An input output error occured");
						break;
					case ENAMETOOLONG: perror("Path is too long");
						break;
					case ENOTDIR: perror("A component of path not a directory"); 
						break;
					case ENOENT: perror("No such file or directory"); printf("enoent\n");
      
					default: printf("Couldn't change directory to %s", par[1] ); 
					}
				}
				
			}
			break;
		case 4:
			printHistory();
			
		}	
		
	
}

void run(){
	int m;
	int intersize = sizeof(inter)/sizeof(char *);
	int in;
	in = 0;

		if(par[0] == NULL){
			printf("No command entered");
		}
	
		for(m = 0; m < intersize; m++){
			if(strcmp(par[0], inter[m]) == 0){
				in = 1;
				runInternal(m);
			}
		}
		if(in == 0) executeCMD();
		memset(par,0,sizeof(par));
		
}

void tokenise(){
	char tokeniser[50] = "\t \n <>&|;";
	char *p = strtok(input, tokeniser);
	int i = 0;
			
	if(NULL == p){
		return;
	}
	
	while(p != NULL){
		par[i] = p;
		++i;
		printf("(%s)\n", p);
		p = strtok(NULL, tokeniser);
	}
	
printf("%s", par[1]);
	run();
}

void historyCommand(){

	char strin[1000];
	char *ptr;
	long ret;
	int choice;
	int i;
	int k = strncmp(input, "!-", 2);
	printf("\n%s\n", input);
if(k!=0){
	strncpy(strin, &input[1],3); //Ignores the first character of input and copies
					// the next 3 chars

	ret = strtol(strin,&ptr,10);
	choice = ret;
	choice--;


	if(histLoop == 0 && choice > 0 && choice <= historyCount){
		
			strcpy(input,history[choice]);
			tokenise();
	}
	else if(histLoop == 1 && choice > 0 && choice < 21){
			strcpy(input,history[choice]);
			tokenise();
	}
	else{
		printf("%s : event not found here 4",input);
	}
}
else{
		strncpy(strin, &input[2],3); //Ignores the first character of input and copies
					// the next 3 chars

	ret = strtol(strin,&ptr,10);
	choice = ret;
	

	if(histLoop == 0 && choice > 0 && choice <= historyCount){
		i = historyCount - choice;
		i--;
		
			if(i > 0){
			strcpy(input,history[i]);
			tokenise();
			}
	}
	else if(histLoop == 1 && choice > 0 && choice < 20){
			i = 20 - choice;
			i--;
			if(i>0){
			strcpy(input,history[i]);
			tokenise();	
			}
	}

}
	
}


void getInput(){
	/*char input[1000];*/
	
	
	while(1){
		printf(pointer);
		if(fgets(input, MAX, stdin) == NULL){
			exit(0);
		}
		int t = strncmp(input, "!", 1);
		int k = strncmp(input, "!-", 2);
		if( t != 0){
		saveHistory();
		}
		if(t == 0){/*
			if(input[3] == '\0' || input[2] == '\0' || input[4] == '\0'){
				if((isdigit(input[1]) && input[2] == '\0') || (isdigit(input[1]) && isdigit(input[2])	)){
			printf("hehEHE111");
			historyCommand();
				}
			}*/



			if(input[3] == '\0'){
				
				if(isdigit(input[1])){
					
					historyCommand();
				}
				else{
					printf("%s : event not found here 1",input);
				}
			}

			else if(input[4] == '\0' && k !=0){
				if(isdigit(input[1]) && isdigit(input[2])){
					
					historyCommand();
				}
				else{
					printf("%s : event not found here 2",input);
				}
			}


			else if(k==0){
				if(input[4]=='\0' && isdigit(input[2])){
					historyCommand();
				}
				else if((input[5]=='\0') && isdigit(input[2]) && isdigit(input[3])){
					historyCommand();
				}
				else{
				printf("%s : event not found here 3",input);
			}

			}
			
			

			else{
				printf("%s : event not found",input);
			}
		}




		
		else tokenise();
	
		}
}

int main(){
const char* path = getenv("PATH");
	home = getenv("HOME");
	chdir(home);
	printf("PATH :%s\n",(path!=NULL)? path : "getenv returned NULL");	
	printf("Home: %s\n ",home); 	
	getInput();
	return(0);
}
