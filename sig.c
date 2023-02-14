#include "headers.h"
#include "sig.h"

void sigjob(char* command)
{
	int i = 0;
	int jobnumber = 0;
	int signalnumber = 0;
	char numbers[2][3];	//signal and job id cannot have more than 2 digits
	numbers[0][0] = '\0';
	numbers[1][0] = '\0';
	int k = 0;
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
			if(k>=2)
			{
				perror("only two arguments allowed");
				return;
			}
			if(kcount>1)
			{
				perror("Invalid argument: No number can be 3 digit");
				return;
			}
			numbers[k][kcount++] = command[i];
			regis = true;
		}
		else if(regis==true)
		{
			numbers[k][kcount] = '\0';
			k++;
			kcount = 0;
			regis = false;
		}
	}
	numbers[k][kcount] = '\0';
	if(numbers[0][0]=='\0')
	{
		perror("job number not entered");
		return;
	}
	if(numbers[1][0]=='\0')
	{
		perror("signal number not entered");
		return;
	}
	jobnumber = atoi(numbers[0]);
	signalnumber = atoi(numbers[1]);
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
	if(kill(pidnum, signalnumber)==-1)
	{
		perror("sig error");
		return;
	}
}
