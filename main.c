#include "execute.h"
#include "pwd.h"
#include "prompt.h"
#include "headers.h"
#include "history.h"
#include "jobs.h"

int keepitup = 1;	//signal handling for ctrl+d, i.e., logging out of shell
int shell_pid = 0;	//ctrl+c handling to check if fg process is shell or not

void child_handler(int dummy)
{
	int i = 0;
	int status;
	pid_t pid;
	/*
	The waitpid() function obtains status information for
    process termination, and optionally process stop and/or continue,
    from a specified subset of the child processes.
    If pid is equal to (pid_t)-1, status is requested for any
    child process.
    
    WNOHANG specifies that waitpid should return immediately
	instead of waiting, if there is no child process ready to be noticed.
	*/
	
	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		for(i = 0; i<64; i++)
		{
			if(list[i].pid==pid)
			{
				if(WIFEXITED(status)!=0)
				{
					printf("%s with pid %d exited normally\n", list[i].command, list[i].pid);
				}
				else
				{
					printf("%s with pid %d exited abnormally\n", list[i].command, list[i].pid);
				}
				list[i].pid = -1;
				list[i].jid = -1;
				strcpy(list[i].command, "\0");
//				list[i].cmd_state = "\0";
				strcpy(list[i].cmd_state, "\0");
//				list[i].command = "\0";
			}
		}
	}
}

void ctrlz_handler(int dummy)
{
	printf("\n");
	fflush(stdout);
	int current_pid = fgjob.pid;
	if(current_pid==-1)
	{
		return;
	}
	return;
	//retrospect
	//might need to remove everything after this
//	addjob(current_pid, fgjob.command);
//	fgjob.pid = -1;
//	strcpy(fgjob.command, "\0");
//	signal(SIGTSTP, SIG_DFL);
//	setpgid(0,0);
//	if(kill(current_pid, SIGSTOP)==-1)
//	{
//		perror("ctrl+z sig error");
//		return;
//	}
//	
//	signal(SIGTTIN, SIG_IGN);	//ignore signal
//	signal(SIGTTOU, SIG_IGN);	//ignore signal
//	
//	if(tcsetpgrp(0,shell_pid)<0)	//set shell as the foreground process
//	{
//		perror("tcsetpgrp error: ctrl+z failed");
//		return;
//	}
//	//need to add it to the bg list
	
//	
//	signal(SIGTTIN, SIG_DFL);
//	signal(SIGTTOU, SIG_DFL);
}

void ctrlc_handler(int dummy)
{
	printf("\n");
	int current_pid = fgjob.pid;
	if(current_pid==-1)
	{
		return;
	}
	return;
	//retrospect
	if(kill(current_pid, SIGINT)==-1)
	{
		perror("ctrl+c sig error");
		return;
	}
}

void mademoishelle()
{
	char* input = (char*)malloc(in_len*sizeof(char));
	int cur_len = in_len;

	
	while(keepitup)
	{
		//SIGNAL HANDLING
		signal(SIGCHLD, child_handler);	//Child handler
		signal(SIGTSTP, ctrlz_handler);	//CTRL+Z
		signal(SIGINT, ctrlc_handler);	//CTRL+C
		///////////
		
		print_prompt();
		
		if(input!=NULL)
		{
			
			int ch = EOF;
			int i = 0;
			int count = 1; //there shall be atleast one command(the empty command, let's say)
			
			while((ch=getchar())!='\n')	// && ch!=EOF
			{
				if(ch==EOF)	//ctrl d signal handling
				{
					keepitup = 0;
					goto ctrld;
					break;
				}
				input[i] = (char)ch;
				if(input[i]==';')
					count++;
				i++; 
				if(cur_len==i)
				{
					cur_len += in_len;
					input = realloc(input, cur_len*sizeof(char));
				}
			}
			input[i] = '\0'; //string terminating character
			int strict_size = strlen(input); // length of input
			
			if(input[0]!='\0')
				enqueue(input); //HISTORY - if command is not empty
			
			int j = 0;
			
			if(count>1)
			{
				char* commands[count];
				char* token;
				token = strtok(input, ";");
	
				while(token!=NULL)
				{
					commands[j] = (char*)malloc((strict_size + 10)*sizeof(char));
					strcpy(commands[j], token);
					j++;
					token = strtok(NULL, ";");
				}
				
				for(i = 0; i<j; i++)
				{
					execute(commands[i]);
	//				free(commands[i]);
				}
			}
			else
			{
				char commands[strict_size+10];
//				printf("WHATS HAPPENINBRUH\n");
//				commands[0] = (char*)malloc(strict_size*sizeof(char));
//				printf("Why\n");
				strcpy(commands, input);
				if(commands[0]=='\0')
					continue;
//				printf("WH!!!!!!\n");
				execute(commands);
			}
			
//			free(token); //??????

			
//			free(commands);
		}
	}
	ctrld:			//ctrld handling
		free(input);
		input = NULL;
		return;
}


int main(int argc, char *argv[])
{
	shell_pid = getpid();
	home_dir = getenv("PWD");
	in_len = 1024; //for input length
	
	front = -1;//for queue implementation of history
	rear = -1;
	int i = 0;
	for(i = 0; i<QSIZE; i++)
	{
		hqueue[i] = NULL;
	}
	//listen up, for history implementation, copy contents of the queue to ".mademoiselle_history" after 
	//session is exited, then for every new session, copy contents from the file to the array of the queue
	// et voila!
	
	prev_dir = (char*)malloc(PATH_MAX*sizeof(char));
	
	for(i=0;i<64;i++)	//creating list of currently running background processes
	{
		list[i].pid = -1;
		list[i].jid = -1;	//job number assigned by index
		strcpy(list[i].command, "\0");
		strcpy(list[i].cmd_state, "\0");
	}
	
	fgjob.pid = -1;	//keeping track of foreground job running on top of the shell
	fgjob.jid = -1;
	strcpy(fgjob.command, "\0");
	strcpy(fgjob.cmd_state, "\0");
	
	mademoishelle(); // :)
	
	free(prev_dir);
	prev_dir = NULL;
	printf("\n");
	
	return 0;
}
