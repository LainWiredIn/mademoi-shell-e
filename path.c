#include "pwd.h"
#include "path.h"
#include "headers.h"

char* print_path()
{
	int i = 0;
//	char* cur_dir = present_working_directory();
	char cur_dir[PATH_MAX];
//	char* cur_dir = (char*)malloc(PATH_MAX*sizeof(char)); //this used to be before
	strcpy(cur_dir, present_working_directory());
	int cur_dir_len = strlen(cur_dir);
	int home_dir_len = strlen(home_dir);
	
	if(cur_dir_len==home_dir_len)
	{
		if(!strcmp(cur_dir, home_dir))
			return "~";
		else
		{
			char* ptr = cur_dir;
			return ptr;
//			return cur_dir;
		}
			
	}
	
	else if(cur_dir_len<home_dir_len)
	{
		char* ptr = cur_dir;
		return ptr;
//		return cur_dir;
	}
	
	else
	{
		char temp[home_dir_len];
		for(i = 0; i<home_dir_len; i++)
		{
			temp[i] = cur_dir[i];
		}
		
		if(!strcmp(temp, home_dir) && cur_dir[home_dir_len]=='/')
		{
			char path[PATH_MAX] = "~/";
		
			for(i = 0; i<(cur_dir_len-home_dir_len); i++)// earlier I had put (cur_dir_len-home_dir_len-1)
			{
				path[i+2] = cur_dir[home_dir_len+1+i];
			}
			char* ptr = path;
			return ptr;
		}
		else
		{
			char* ptr = cur_dir;
			return ptr;
			//return cur_dir;
		}
	}
}
