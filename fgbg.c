#include "headers.h"
#include "fgbg.h"

void fg(char* command)
{
	int i;
	char numbers[3];
	numbers[0] = '\0';
	int kcount = 0;
	bool regis = false;
	for(i = 0;i<strlen(command);i++)
	{
		if(command[i]!=' ' && command[i]!='\t')
		{
			if(command[i] < 48 || command[i] > 57)
			{
				perror("invalid input");
				return;
			}
			if(kcount>1)
			{
				perror("Invalid argument: No job id can be 3 digit");
				return;
			}
			numbers[kcount++] = command[i];
			regis = true;
		}
		else if(regis==true)
		{
			break;
		}
	}
	numbers[2] = '\0';
	int jobnumber = atoi(numbers);
	
	pid_t pidnum = -1;
	for(i = 0;i<64;i++)
	{
		if(list[i].jid==jobnumber)
		{
			pidnum = list[i].pid;
			break;
		}
	}
	if(pidnum==-1)
	{
		perror("JOB NOT FOUND\n");
		return;
	}
	if(kill(pidnum, SIGCONT)==-1)
	{
		perror("fg: sig error");
		return;
	}
	
	signal(SIGTTIN, SIG_IGN);	//ignore signal
	signal(SIGTTOU, SIG_IGN);	//ignore signal
	
	if(tcsetpgrp(0,pidnum)<0)
	{
		perror("tcsetpgrp error: fg failed");
		return;
	}
	int status;
	waitpid(pidnum, &status, WUNTRACED);
	if(tcsetpgrp(0,getpgid(getpid()))<0)
	{
		perror("tcsetpgrp error: fg failed");
		return;
	}
//	printf("%s\n", list[i].command);

	fgjob.pid = list[i].pid;
	strcpy(fgjob.command, list[i].command);
	
	list[i].pid = -1;
	list[i].jid = -1;
	strcpy(list[i].command, "\0");
	strcpy(list[i].cmd_state, "\0");
		
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	
}

void bg(char* command)
{
	int i;
	char numbers[3];
	numbers[0] = '\0';
	int kcount = 0;
	bool regis = false;
	for(i = 0;i<strlen(command);i++)
	{
		if(command[i]!=' ' && command[i]!='\t')
		{
			if(command[i] < 48 || command[i] > 57)
			{
				perror("invalid input");
				return;
			}
			if(kcount>1)
			{
				perror("Invalid argument: No jobid can be 3 digit");
				return;
			}
			numbers[kcount++] = command[i];
			regis = true;
		}
		else if(regis==true)
		{
			break;
		}
	}
	numbers[2] = '\0';
	int jobnumber = atoi(numbers);
	
	pid_t pidnum = -1;
	for(i = 0;i<64;i++)
	{
		if(list[i].jid==jobnumber)
		{
			pidnum = list[i].pid;
			break;
		}
	}
	if(pidnum==-1)
	{
		perror("JOB NOT FOUND\n");
		return;
	}
	if(kill(pidnum, SIGCONT)==-1)
	{
		perror("bg: sig error");
		return;
	}
}
