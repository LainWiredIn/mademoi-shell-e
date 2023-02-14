#include "cd.h"
#include "pwd.h"
#include "headers.h"

void change_dir(char* command)
{
	int i = 0, j = 0;
	
	if(command=="\0")
	{
		char *temp = (char*)malloc(PATH_MAX*sizeof(char));
		strcpy(temp, present_working_directory());
		for(i = 0; i<strlen(temp); i++)
		{
			prev_dir[i] = temp[i];
		}
		prev_dir[i] = '\0';
		if(chdir(home_dir)!=0)
		{
			perror("chdir failed");
		}
		free(temp);
		temp = NULL;
		return;
	}
	
	int size = strlen(command);
	char argument[size];
	
	int empty = 0;
	bool check = false;
	for(i = 0;i<size;i++) //assuming path has no spaces
	{
		if(command[i]!=' ' && command[i]!='\t')
		{
			argument[j] = command[i];
			j++;
			if(check==false)
			{
				check = true;
				empty++;
			}
		}
		else
		{
			check = false;
		}
	}
	
	if(empty==0) //no argument present
	{
//		memset(prev_dir, 0, strlen(prev_dir));
		char *temp = (char*)malloc(PATH_MAX*sizeof(char));
		strcpy(temp, present_working_directory());
		for(i = 0; i<strlen(temp); i++)
		{
			prev_dir[i] = temp[i];
		}
		prev_dir[i] = '\0';
		if(chdir(home_dir)!=0)
		{
			perror("chdir failed");
		}
		free(temp);
		temp = NULL;
		return;
	}
	
	else if(empty>1)
	{
		perror("More than one path not accepted");
		return;
	}
	
	argument[j] = '\0';
	
	if(!strcmp(argument, "~"))
	{
//		memset(prev_dir, 0, strlen(prev_dir));
		char *temp = (char*)malloc(PATH_MAX*sizeof(char));
		strcpy(temp, present_working_directory());
		for(i = 0; i<strlen(temp); i++)
		{
			prev_dir[i] = temp[i];
		}
		prev_dir[i] = '\0';
		if(chdir(home_dir)!=0)
		{
			perror("chdir failed");
		}
		free(temp);
		temp = NULL;
		return;
	}
	
	else if(!strcmp(argument, "-"))
	{
		if(prev_dir[0]=='\0')
		{
			printf("bash: cd: OLDPWD not set\n");
			return;
		}
		else
		{
			char *temp = (char*)malloc(PATH_MAX*sizeof(char));
			strcpy(temp, present_working_directory());
			char prev_change[strlen(temp)+1];
			for(i = 0; i< strlen(temp); i++)
			{
				prev_change[i] = temp[i];
			}
			prev_change[strlen(temp)] = '\0';
			if(chdir(prev_dir)!=0)
			{
				perror("chdir failed");
				free(temp);
				temp = NULL;
				return;
			}
			printf("%s\n", present_working_directory());
//			memset(prev_dir, 0, strlen(prev_dir));
			for(i = 0; i<strlen(prev_change); i++)
			{
				prev_dir[i] = prev_change[i];
			}
			prev_dir[i] = '\0';
			free(temp);
			temp = NULL;
			return;
		}
	}
	
	else
	{
//		memset(prev_dir, 0, strlen(prev_dir));
		char *temp = (char*)malloc(PATH_MAX*sizeof(char));
		strcpy(temp, present_working_directory());
		for(i = 0; i<strlen(temp); i++)
		{
			prev_dir[i] = temp[i];
		}
		prev_dir[i] = '\0';
		if(chdir(argument)!=0)
		{
			perror("chdir failed");
			return;
		}
		free(temp);
		temp = NULL;
		return;
	}
}
