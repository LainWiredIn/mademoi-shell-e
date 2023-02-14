#include "execute.h"
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

void execute(char* command)
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
    /////
    
	i = 0;
	char argument[2][1024];
	int j = 0;
	bool check = false;
	
	int comm_size = strlen(command) + 10;
	
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
	
	int pipe = 0;
	
	for(i = 0; i<comm_size; i++)
	{
		if(j==1) //copy arguments for each command into a separate array
		{
			temp[k++] = command[i];
			if(command[i]=='|')
				pipe++;
			continue;
		}
		if(command[i]==' ' || command[i]=='\t')
		{
			if(check==true) //for ignoring spaces before command is entered
			{
				check = false;
				temp[k] = '\0';
//				argument[j] = (char*)malloc((strlen(temp) + 2)*sizeof(char));
				strcpy(argument[j], temp);
				k = 0;
				temp[0] = '\0';
				j++;
			}
		}
		else
		{
			temp[k++] = command[i];
			if(command[i]=='|')
				pipe++;
			check = true;
		}
	}
	
	if(pipe>0)
	{
		pipe_cmd(pipe+1, command);
//		free(temp);
//		temp = NULL;
		return;
	}
	
	strcpy(argument[1], "\0");
	if(check==true) //command found with no spaces after it
	{
		temp[k] = '\0';
//		argument[j] = (char*)malloc((strlen(temp) + 2)*sizeof(char));
		strcpy(argument[j], temp);
//		free(temp);
//		temp = NULL;
	}
	else if(j==1)
	{
		temp[k] = '\0';
//		argument[j] = (char*)malloc((strlen(temp) + 2)*sizeof(char));
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
		argument[1][stopping_index] = '\0';	//both input and output can be used on only one file/exe at a time
		redirection_check = true;
		printf("");
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
		char subargs[2][strlen(argument[1]) + 10];
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
		//SYSTEM COMMANDS
		sys_comms(argument[0], argument[1]);
	}
	
	if(redirection_check==true)	//reset file descriptors to stdin and stdout
	{
		dup2(original_output, STDOUT_FILENO);
		close(original_output);
		dup2(original_input, STDIN_FILENO);
		close(original_input);
	}
//	else
//	{
//		printf("bash: %s: command not found\n", argument[0]);
//	}

}
