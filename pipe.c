#include "headers.h"
#include "pipe.h"
#include "execute.h"
////////////////////
#include "pwd.h"
#include "cd.h"
#include "echo.h"
#include "pinfo.h"
#include "prompt.h"
#include "syscom.h"
#include "ls.h"
#include "history.h"
#include "headers.h"
#include "pipe.h"
#include "jobs.h"
#include "sig.h"
#include "fgbg.h"
///////////////////

void pipe_execution(char* command)
{
	int i = 0;
    int index_for_trailing_spaces = -1;
	
	//handling trailing spaces
    while(command[i]!='\0')
    {
        if(command[i]!=' ' && command[i]!='\t' && command[i]!='\n')
        {
            index_for_trailing_spaces = i;
        }
        i++;
    }
    if(index_for_trailing_spaces!=-1)
    	command[index_for_trailing_spaces + 1] = '\0';
    	
    i = 0;
	char argument[2][1024];
	int j = 0;
	bool check = false;
	
	int comm_size = strlen(command);
	
	//CHECK FOR EMPTY COMMANDS
	bool allspaces = true;
	for(i = 0;i<comm_size;i++)
	{
		if(command[i]!=' ' && command[i]!='\t')
		{
			allspaces = false;
			break;
		}
	}
	if(allspaces==true)
	{
		return; //COMMAND is empty
	}
	///////////////////
	
	char temp[1024];
	int k = 0;
	
	for(i = 0; i<comm_size; i++)
	{
		if(j==1) //copy arguments for each command into a separate array
		{
			temp[k++] = command[i];
			continue;
		}
		if(command[i]==' ' || command[i]=='\t')
		{
			if(check==true) //for ignoring spaces before command is entered
			{
				check = false;
				temp[k] = '\0';
//				argument[j] = (char*)malloc(strlen(temp)*sizeof(char));
				strcpy(argument[j], temp);
				k = 0;
				temp[0] = '\0';
				j++;
			}
		}
		else
		{
			temp[k++] = command[i];
			check = true;
		}
	}
	
	strcpy(argument[1], "\0");
	if(check==true) //command found with no spaces after it
	{
		temp[k] = '\0';
//		argument[j] = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(argument[j], temp);
//		free(temp);
//		temp = NULL;
	}
	else if(j==1)
	{
		temp[k] = '\0';
//		argument[j] = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(argument[j], temp);
//		free(temp);
//		temp = NULL;
	}
	
	//HANDLING REDIRECTION
	int len = strlen(argument[1]);
	
	char input_file[1024];
	int ifk = 0;
	int fd_input;
	char output_file[1024];
	int ofk = 0;
	int fd_output;
	
	int original_input = dup(STDIN_FILENO);
	int original_output = dup(STDOUT_FILENO);
	
	int stopping_index = -1;
	
	for(i = 0;i<len;i++)
	{
		if(argument[1][i]=='<')
		{
			if(stopping_index==-1)
				stopping_index = i;
			i++;
			while(1)
			{
				if(argument[1][i]=='\0' || argument[1][i]=='>')
					break;
				if(argument[1][i]!=' ' && argument[1][i]!='\t') //filename has no spaces and tabs
				{
					input_file[ifk++] = argument[1][i];
				}
				i++;
			}
			
			input_file[ifk] = '\0';
			
			//open file for read
			fd_input = open(input_file, O_RDONLY);
			if(fd_input<0) //error handling
			{
				perror("input file error");
				return;
			}
			if(dup2(fd_input, STDIN_FILENO) < 0) 
			{
		    	perror("Unable to duplicate file descriptor");
		        return;
    		}
    		close(fd_input);
    		ifk = 0;
		}
		if(argument[1][i]=='>')
		{
			if(stopping_index==-1)
				stopping_index = i;
			if(i!=(len-1) && argument[1][i+1]=='>')
			{
				i = i+2;
				while(1)
				{
					if(argument[1][i]=='\0' || argument[1][i]=='<')
						break;
					if(argument[1][i]!=' ' && argument[1][i]!='\t') //filename has no spaces and tabs
					{
						output_file[ofk++] = argument[1][i];
					}
					i++;
				}
				
				output_file[ofk] = '\0';
				
				fd_output = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644); //DON'T TRUNCATE JUST APPEND
				if(fd_output<0) //error handling
				{
					perror("output file error");
					return;
				}
				if(dup2(fd_output, STDOUT_FILENO) < 0) 
				{
			    	perror("Unable to duplicate file descriptor");
			        return;
	    		}
	    		close(fd_output);
	    		ofk = 0;
			}
			else
			{
				// | O_TRUNC
				i++;
				while(1)
				{
					if(argument[1][i]=='\0' || argument[1][i]=='<')
						break;
					if(argument[1][i]!=' ' && argument[1][i]!='\t') //filename has no spaces and tabs
					{
						output_file[ofk++] = argument[1][i];
					}
					i++;
				}
				
				output_file[ofk] = '\0';
				
				fd_output = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); //TRUNCATE NOT APPEND
				if(fd_output<0) //error handling
				{
					perror("output file error");
					return;
				}
				if(dup2(fd_output, STDOUT_FILENO) < 0) 
				{
			    	perror("Unable to duplicate file descriptor");
			        return;
	    		}
	    		close(fd_output);
	    		ofk = 0;
			}
		}
	}
	
	bool redirection_check = false;
	if(stopping_index!=-1)
	{
//		for(i = 0;i<len;i++)
//		{
//			if(argument[1][i]=='>' || argument[1][i]=='<')
//				argument[1][i] = ' ';
//		}
		argument[1][stopping_index] = '\0';	//assuming only one file/exe
		redirection_check = true;
	}
	
	//////////////////////////////////
	
	
	if(!strcmp(argument[0], "pwd")) //present working directory
	{
		printf("%s\n", present_working_directory());
	}
	
	else if(!strcmp(argument[0], "echo"))	//write to stdout
	{
		echo_echo(argument[1]);
	}
	
	else if(!strcmp(argument[0], "cd"))
	{
		change_dir(argument[1]);
	}
	
	else if(!strcmp(argument[0], "ls"))
	{
		list_dir(argument[1]);
	}
	
	else if(!strcmp(argument[0], "pinfo"))
	{
		pinfo(argument[1]);
	}
	
	else if(!strcmp(argument[0], "jobs"))
	{
		displayjobs(argument[1]);
	}
	
	else if(!strcmp(argument[0], "sig"))
	{
		sigjob(argument[1]);
	}
	
	else if(!strcmp(argument[0], "fg"))
	{
		fg(argument[1]);
	}
	
	else if(!strcmp(argument[0], "bg"))
	{
		bg(argument[1]);
	}
	
	else if(!strcmp(argument[0], "history"))
	{
		int number = -1;
		if(argument[1]!="\0")
			number = atoi(argument[1]);
		display(number);
	}
	
	else if(!strcmp(argument[0], "repeat"))
	{
		int i = 0;
		char subargs[2][strlen(argument[1])+1];
//		subargs[0] = (char*)malloc(strlen(argument[1])*sizeof(char));
//		subargs[1] = (char*)malloc(strlen(argument[1])*sizeof(char));
		bool checker = false;
		bool help = false;
		int k = 0;
		for(i = 0; i<strlen(argument[1]); i++)
		{
			if(help==true)
			{
				subargs[1][k] = argument[1][i];
				k++;
			}
			else if(argument[1][i]==' ' || argument[1][i]=='\t')
			{
				if(checker==true)
				{
					help = true;
					k = 0;
				}
			}
			else if(argument[1][i]!='0' && argument[1][i]!='1' && argument[1][i]!='2' && argument[1][i]!='3' && argument[1][i]!='4' && argument[1][i]!='5' && argument[1][i]!='6' && argument[1][i]!='7' && argument[1][i]!='8' && argument[1][i]!='9')
			{
				perror("Wrong input\n");
				return;
			}
			else
			{
				subargs[0][k] = argument[1][i];
				k++;
				checker = true;
			}
		}
		subargs[1][k] = '\0';
		
		int rep = atoi(subargs[0]);
		
		for(i = 0; i<rep; i++)
		{
			execute(subargs[1]);
		}		
	}
	
	else if(!strcmp(argument[0], "exit") || !strcmp(argument[0], "quit"))
	{
		//implement some history copy into file here
		exit(1);
	}
	else
	{
// 		don't run syscom here
//		sys_comms(argument[0], argument[1]);
		//parsing required
		fflush(stdout);
		int i = 0;
		char command[1024];
		
		strcpy(command, argument[0]);
		char arguments[1024];
		if(argument[1][0]=='\0')
		{
			arguments[0] = '\0';
		}
		else
		{
			int k = 0;
			bool tmp = false;
			for(i = 0; i<strlen(argument[1]); i++)
			{
				if((argument[1][i]==' ' || argument[1][i]=='\t') && tmp==false)//space handling
				{
					//do nothing
				}
				else if(tmp==true)
				{
					arguments[k] = argument[1][i];
					k++;
					tmp = false;
				}
				else
				{
					arguments[k] = argument[1][i];
					k++;
					tmp = true;
				}
			}
		}
		
		pid_t syspid;
		char *argv[3];
		if((syspid = fork()) == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if(syspid==0)
		{
			if(arguments[0]=='\0')
			{
				argv[0] = command;
				argv[1] = NULL;
				argv[2] = NULL;
//				strcpy(argv[0], command);
//				strcpy(argv[1], NULL);
//				strcpy(argv[2], NULL);
//				execvp(argv[0], argv);
				if(execvp(argv[0], argv)==-1)	//or less than zero?
				{
					perror("exec Error in pipe");
					exit(EXIT_FAILURE);
				}
        		exit(EXIT_FAILURE);
			}
			else
			{
				argv[0] = command;
				argv[1] = arguments;
				argv[2] = NULL;
//				char *argv[] = {command, arguments, NULL};
//				strcpy(argv[0], command);
//				strcpy(argv[1], arguments);
//				strcpy(argv[2], NULL);
				if(execvp(argv[0], argv)==-1)	//or less than zero?
				{
					perror("exec Error in pipe");
					exit(EXIT_FAILURE);
				}
        		exit(EXIT_FAILURE);
			}
//			char *argv[] = {command, arguments, NULL};
//			execvp(argv[0], argv);
//        	exit(EXIT_FAILURE);
		}
		else
		{
			wait(NULL);
			free(argv[0]);
			free(argv[1]);
			free(argv[2]);
			argv[0] = NULL;
			argv[1] = NULL;
			argv[2] = NULL;
		}
		
	}
	
	if(redirection_check==true)	//reset file descriptors to stdin and stdout
	{
		dup2(original_output, STDOUT_FILENO);
		close(original_output);
		dup2(original_input, STDIN_FILENO);
		close(original_input);
	}
}



void pipe_cmd(int n, char* input)
{
	int original_input = dup(STDIN_FILENO);
	int original_output = dup(STDOUT_FILENO);
	
	char commands[n][2048];
	int j = 0, i = 0;
	
	int strict_size = strlen(input) + 10;
	char* token;
	token = strtok(input, "|");

	while(token!=NULL)
	{
//		commands[j] = (char*)malloc(strict_size*sizeof(char));
		strcpy(commands[j], token);
		
		//REMOVING TRAILING SPACES
		i = 0;
	    int index_for_trailing_spaces = -1;
	    while(commands[j][i]!='\0')
	    {
	        if(commands[j][i]!=' ' && commands[j][i]!='\t' && commands[j][i]!='\n')
	        {
	            index_for_trailing_spaces = i;
	        }
	        i++;
	    }
	    commands[j][index_for_trailing_spaces + 1] = '\0';
	    ///////////////////////////
//	    printf("%s is the command received\n", commands[j]);
		j++;
		token = strtok(NULL, "|");
	}
	
	fflush(stdout);
	int p[2];
	pid_t pid;
	int fd_in = 0;

	for(i = 0;i<n;i++)
	{
		pipe(p);
		if((pid = fork()) == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if(pid==0)
		{
			dup2(fd_in, 0);
			if(i!=(n-1))
			{
				dup2(p[1], 1);
			}
			close(p[0]);
			pipe_execution(commands[i]);
		}
		else
		{
			wait(NULL);
			close(p[1]);
			fd_in = p[0];
		}
	}
	
	
	/* code snippet 20
	
	int pipefd[2];
	int childpid,childpid2;
	char* cmd[3]={"cat","1.txt",NULL};	//command[0]
	char* cmd2[2]={"wc",NULL};	//command[1]
	pipe(pipefd);
	if(childpid=fork())
	{
	   	//parent
	   	close(pipefd[1]);
	   	dup2(pipefd[0],STDIN_FILENO);
//  		execute(commands[1]);
		execvp("cat", cmd);
	}
	else
	{
	   	//child
	   	//write
	   	close(pipefd[0]);
	   	dup2(pipefd[1],STDOUT_FILENO);
//	   	execute(commands[0]);
	   	execvp("wc", cmd);
	}
	
	*///code snippet 20 ends
	
	/*
	
	
	
	int in = dup(0); //stdin
	int out = dup(1); //stdout
//	int fd[2];
	pid_t pid;
	int status, prev, next;
	
	for(i = 0;i<n;i++)
	{
		if(i==0)
		{
			prev = 0;
			next = 1;
		}
		else if(i==(n-1))
		{
			prev = 1;
			next = 0;
		}
		else
		{
			prev = 1;
			next = 1;
		}
		if(prev)
		{
			close(fildes[1]);
			perror("write end closed\n");
			in = dup(0);
			if(dup2(fildes[0], 0) < 0)
			{
				perror("dup2 error");
			}
			close(fildes[0]);
			perror("read end closed\n");
			if(i==(n-1))
			{
				dup2(original_output, 1);
			}
			
		}
		if(next)
		{
			pipe(fildes);
			out = dup(1);
			if(dup2(fildes[1], 1) < 0)
			{
				perror("dup2 error");
			}
		}
		pid = fork();
		if(pid<0)
		{
			perror("pid error");
			exit(EXIT_FAILURE);
		}
		else if(pid==0)
		{
			piped_execute(commands[i]);
			char cmmd[1024];
			strcpy(cmmd, "executed ");
			strcat(cmmd, commands[i]);
			strcat(cmmd, "\n");
			perror(cmmd);
		}
		else
		{
			waitpid(pid, &status, WUNTRACED);
			dup2(in, 0);
			dup2(out, 1);	
		}
		
		
		*/
		
		
//		pipe(fd);
//		pid = fork();
//		if(pid<0)
//		{
//			perror("pid error");
//			exit(EXIT_FAILURE);
//		}
//		else if(pid==0)
//		{
//			dup2(in, STDIN_FILENO);
//			if(i!=(n-1))
//			{
//				dup2(fd[1], STDOUT_FILENO);
//			}
//			else
//			{
//				dup2(original_output, STDOUT_FILENO);
//			}
//			close(fd[0]);
//			piped_execute(commands[i]);
//		}
//		else
//		{
//			wait(NULL);
//			fflush(stdout);
//			close(fd[1]);
//			in = fd[0];
//		}
//	}
//	dup2(original_input, STDIN_FILENO);
//	dup2(original_output, STDOUT_FILENO);

	
	
//	for(i = 0;i<n-1;i++)
//	{
//		if(pipe(fd)<0)
//		{
//			perror("Pipe");
//			break;
//		}
//		
//		pid = fork();
//		if(pid==-1)
//		{
//			perror("Failed to fork, exiting");
//			exit(1);
//		}
//		else if(pid==0)
//		{
//			dup2(in, 0);
//			if(i!=(n-1))
//				dup2(fd[1],1);
////			else
////			{
////				dup2(original_output, 1);
////				close(original_output);
////			}
//			close(fd[0]);
//			execute(commands[i]);
//		}
//		else
//		{
//			wait(NULL);
//			close(fd[1]);
//			in = fd[0];
//		}
//		
//		
////		pipe_execution(in, fd[1], commands[i]);
//		
////		close(fd[1]);
////		in = fd[0];
//		
//	}
	
//	dup2(original_output, STDOUT_FILENO);
//	close(original_output);
	
//	if(in!=0)
//	{
//		if(dup2(in, STDIN_FILENO) < 0) 
//		{
//		   	perror("Unable to duplicate file descriptor");
//		    return;
//	   	}
//		close(in);
//	}
//	
//	execute(commands[n-1]);
	
//	dup2(original_input, STDIN_FILENO);
//	close(original_input);
}
