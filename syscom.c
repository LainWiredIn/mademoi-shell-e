#include "syscom.h"
#include "headers.h"
#include "jobs.h"

//int pid_exit = -1;
//char process_exit[PATH_MAX];

//void proc_exit()
//{
//	wait(NULL);
//	printf("%s with pid %d exited normally\n", process_exit, pid_exit);
//}
//might have to write a handler for ctrl c NO NEED

void sys_comms(char* comm, char* args)
{
	int i = 0;
	bool bg = false;
	char command[1024];
	
	strcpy(command, comm);
//	printf("");
//	char* arguments = (char*)malloc(2*sizeof(char));
	char arguments[1024];
	if(args[0]=='\0')
	{
//		arguments = NULL;
		arguments[0] = '\0';
	}
	else
	{
//		arguments = (char*)realloc(arguments, 1024);
		int k = 0;
//		bool tmp = false;
		for(i = 0; i<strlen(args); i++)
		{
			if((args[i]==' ' || args[i]=='\t'))//space handling
			{
				//do nothing
			}
//			else if(tmp==true)
//			{
//				arguments[k] = args[i];
//				k++;
////				argument[k] = '\0';
//				tmp = false;
//			}
			else if(args[i]=='&')
			{
				bg = true;
				//ERROR HANDLING??
				arguments[i] = '\0'; //assuming that & appears at the end
				break;
			}
			else
			{
				arguments[k] = args[i];
				k++;
				arguments[k] = '\0';	//assuming only one argument to system commands
//				tmp = true;
			}
		}
		
		i = 0;
	    int index_for_trailing_spaces = -1;
		
		//handling trailing spaces
	    while(arguments[i]!='\0')
	    {
	        if(arguments[i]!=' ' && arguments[i]!='\t' && arguments[i]!='\n')
	        {
	            index_for_trailing_spaces = i;
	        }
	        i++;
	    }
	    if(index_for_trailing_spaces!=-1)
	    	arguments[index_for_trailing_spaces + 1] = '\0';
	    /////
	}
	
	if(bg==false) //RUN FOREGROUND PROCESS
	{
		char *argv[3];
		pid_t pid = fork();
		
		if(pid<0)
		{
			perror("Failed to fork, exiting");
			exit(1);
		}
		
		else if(pid==0)
		{
			signal(SIGCHLD, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);	//CTRL+Z
			signal(SIGINT, SIG_DFL);	//CTRL+C
			
//			signal(SIGTSTP, SIG_IGN);	//CTRL+Z
//			signal(SIGINT, SIG_IGN);	//CTRL+C
			setpgid(0,0);
			
			
			
//			argv[] = {command, arguments, NULL};
			if(arguments[0]=='\0')
			{
				argv[0] = command;
				argv[1] = NULL;
				argv[2] = NULL;
			}
			else
			{
				argv[0] = command;
				argv[1] = arguments;
				argv[2] = NULL;
			}
			
//			printf("Child pid = %d\n",getpid());  //THIS IS COMPULSORY DON'T REMOVE - no it is compulsory only for bg processes
//			int check = execvp(argv[0], argv);
//			fflush(stdout);
			if((execvp(argv[0], argv))<0)
			{
//				IF ERROR OCCURS, DOES THE PARENT FREEZE?
				perror("exec Error in syscom FG"); //could also be no command
				exit(1);
//				return;
			}
			exit(127);
		}
		
		else	//parent
		{
			fgjob.pid = pid;
			strcpy(fgjob.command, command);
			strcat(fgjob.command, " ");
			strcat(fgjob.command, args);
			
			addjob(fgjob.pid, fgjob.command);
			
			fgjob.pid = -1;
			strcpy(fgjob.command, "\0");
			
			pid_t childpid = pid;
			pid_t shellpid = getpgid(getpid());
			
			signal(SIGTTIN, SIG_IGN);	//ignore signal
			/*
			When any process in a background job tries to read from the terminal, all of the processes in the job 
			are sent a SIGTTIN signal. 
			The default action for this signal is to stop the process.
			*/
			signal(SIGTTOU, SIG_IGN);	//ignore signal
			/*
			generated when a process in a background job attempts to write to the terminal or set its modes. 
			Again, the default action is to stop the process.
			*/
//			signal(SIGCHLD, SIG_DFL);
			/*
			By convention, the process group ID of a 
			process group equals the process ID of the 
			first member of the process group.*/	
			if(tcsetpgrp(0,childpid)<0)
			{
//				int error = errno;
//				printf("error number = %d\n");
//				perror("where?? tcsetpgrp error");
			}
			int status;
			waitpid(childpid, &status, WUNTRACED);
			if(tcsetpgrp(0,shellpid)<0)
			{
//				perror("");//here? tcsetpgrp error
			}
			
			signal(SIGTTIN, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);
			
			free(argv[0]);
			free(argv[1]);
			free(argv[2]);
			argv[0] = NULL;
			argv[1] = NULL;
			argv[2] = NULL;
			
//			if(arguments[0]!='\0')
//			{
//				strcat(command, " ");
//				strcat(command, arguments);
//			}
//			addjob(pid, command);
//			if(WIFSTOPPED(status))
//			{
//				printf("\nSTOPPED\n");
//			}
		}	
	}
	else	//background
	{
		char *argv[3];
		if(arguments[i-1]==' ') //need to handle spaces if background process
		{
			int ij = 0;
		    int index_for_trailing_spaces_v2 = -1;
			
			//handling trailing spaces
		    while(arguments[ij]!='\0')
		    {
		        if(arguments[ij]!=' ' && arguments[ij]!='\t' && arguments[ij]!='\n')
		        {
		            index_for_trailing_spaces_v2 = ij;
		        }
		        ij++;
		    }
		    if(index_for_trailing_spaces_v2!=-1)
		    	arguments[index_for_trailing_spaces_v2 + 1] = '\0';
		}
		pid_t pid = fork();
		
		if(pid<0)
		{
			perror("Failed to fork, exiting");
			exit(1);
		}
		
		else if(pid==0)
		{
			
//			signal(SIGCHLD, SIG_DFL);
//			signal(SIGTSTP, SIG_DFL);	//CTRL+Z
//			signal(SIGINT, SIG_DFL);	//CTRL+C
			
			setpgid(0,0);
			
//			char *argv[] = {command, arguments, NULL};
			if(arguments[0]=='\0')
			{
				argv[0] = command;
				argv[1] = NULL;
				argv[2] = NULL;
			}
			else
			{
				argv[0] = command;
				argv[1] = arguments;
				argv[2] = NULL;
			}

//			int check = execvp(argv[0], argv);
			fflush(stdout);
			if(execvp(argv[0], argv)==-1)	//or less than zero?
			{
				perror("exec Error in syscom BG");
				exit(1);
			}
			exit(127);
		}
		
		else //NEED TO INCLUDE SOME FUNCTIONALITY do i anymore?
		{
//			pid_exit = pid;
//			strcpy(process_exit, command);
			int ult_len = strlen(command) + strlen(arguments) + 2;
			char tempcomm[ult_len];
			strcpy(tempcomm, command);
			if(arguments[0]!='\0')
			{
				strcat(tempcomm, " ");
				strcat(tempcomm, arguments);
			}
			addjob(pid, tempcomm);
			printf("%d\n",pid);
//			printf("%d\n",pid);
//			signal(SIGCHLD, proc_exit);
			return;
		}
	}
}

