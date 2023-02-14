#include "pwd.h"
#include "headers.h"

char *present_working_directory(void)
{
	static char cwd[PATH_MAX];
	if(getcwd(cwd, sizeof(cwd))!=NULL)
	{
		return cwd;
	}
	else
	{
		perror("Couldn't find present working directory");
		return "";
	}
}
