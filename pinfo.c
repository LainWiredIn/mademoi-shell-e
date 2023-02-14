#include "pinfo.h"
#include "headers.h"

void pinfo(char* command)
{
	int pgrp = -100;
	if(command[0]=='\0')
	{
		int pid = getpid();
		printf("pid -- %d\n", pid);
		char path1[1024];
		char path2[1024];
		sprintf(path1, "/proc/%d/stat", pid); //PID(1), STATE(3) memory(23) 
		sprintf(path2, "/proc/%d/exe", pid); //executable address
		FILE* fptr1 = fopen(path1, "r");
		if(fptr1==NULL)
		{
			perror("Error opening /proc/pid/stat file");
		}
		else
		{
			char ch;
			int i = 0;
			pgrp = -100;
			do
			{				
				ch = fgetc(fptr1);
//				if(temppid==-100)
//					temppid = ch - '0';
				if(ch==' ')
				{
					i++;
					if(i==2)
					{
						ch = fgetc(fptr1);
						printf("Process Status -- %c", ch);
					}
					else if(i==4)	//foreground processes are those whose process group id (pgid) is the foreground one on the terminal(tpgid)
					{
						ch = fgetc(fptr1);
						pgrp = ch - '0';
					}
					else if(i==7) //for status checking
					{
						ch = fgetc(fptr1);
						if(pgrp==(ch - '0'))
						{
							printf("+");
						}
						printf("\n");
					}
					else if(i==22)
					{
						printf("memory -- ", ch);
						while((ch = fgetc(fptr1))!=' ')
						{
							printf("%c", ch);
						}
						printf("\n");
						break;
					}
				}
				
			} while(ch != EOF);
		}
		FILE* fptr2 = fopen(path2, "r");
		if(fptr2==NULL)
		{
			perror("Error opening /proc/pid/exe file");
		}
		else
		{
			ssize_t nbytes;
			char buffer[PATH_MAX];
			buffer[0] = '\0';
			nbytes = readlink(path2, buffer, PATH_MAX);
			if (nbytes == -1) 
			{
               perror("readlink error");
               return;
           	}
           	printf("Executable Path -- %s\n", buffer);
           	memset(buffer, 0, strlen(buffer));
		}
	}
	else
	{
		int i = 0, j = 0;
		char arrpid[1024];
		for(i = 0; i<strlen(command); i++)
		{
			if(command[i]==' ' || command[i]=='\t')
			{
				//do nothin'
			}
			
			else if(command[i]!='0' && command[i]!='1' && command[i]!='2' && command[i]!='3' && command[i]!='4' && command[i]!='5' && command[i]!='6' && command[i]!='7' && command[i]!='8' && command[i]!='9')
			{
				printf("Invalid PID\n");
				return;
			}
			else
			{
				arrpid[j] = command[i];
				j++;
			}
		}
		int pid = atoi(arrpid);
		printf("pid -- %d\n", pid);
		char path1[1024];
		char path2[1024];
		sprintf(path1, "/proc/%d/stat", pid); //PID(1), STATE(3) memory(23) 
		sprintf(path2, "/proc/%d/exe", pid); //executable address
		FILE* fptr1 = fopen(path1, "r");
		if(fptr1==NULL)
		{
			perror("Error opening /proc/pid/stat file");
		}
		else
		{
			char ch;
			int i = 0;
			pgrp = -100;
			do
			{				
				ch = fgetc(fptr1);
//				if(temppid==-100)
//					temppid = ch - '0';
				if(ch==' ')
				{
					i++;
					if(i==2)
					{
						ch = fgetc(fptr1);
						printf("Process Status -- %c", ch);
					}
					else if(i==4)
					{
						ch = fgetc(fptr1);
						pgrp = ch - '0';
					}
					else if(i==7) //for status checking
					{
						ch = fgetc(fptr1);
						if(pgrp==(ch - '0'))
						{
							printf("+");
						}
						printf("\n");
					}
					else if(i==22)
					{
						printf("memory -- ", ch);
						while((ch = fgetc(fptr1))!=' ')
						{
							printf("%c", ch);
						}
						printf("\n");
						break;
					}
				}
				
			} while(ch != EOF);
		}
		
		FILE* fptr2 = fopen(path2, "r");
		if(fptr2==NULL)
		{
			perror("Error opening /proc/pid/exe file");
		}
		else
		{
			ssize_t nbytes;
			char buffer[PATH_MAX];
			buffer[0] = '\0';
			nbytes = readlink(path2, buffer, PATH_MAX);
			if (nbytes == -1) 
			{
               perror("readlink error");
               return;
           	}
           	printf("Executable Path -- %s\n", buffer);
           	memset(buffer, 0, strlen(buffer));
		}
	}
}
