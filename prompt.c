#include "prompt.h"
#include "path.h"
#include "headers.h"

void print_prompt()
{
	char system_name[HOST_NAME_MAX];
	char user_name[LOGIN_NAME_MAX];
	gethostname(system_name, HOST_NAME_MAX);
	getlogin_r(user_name, LOGIN_NAME_MAX);
	
	printf("<%s@%s:%s> ", user_name, system_name, print_path());
}
