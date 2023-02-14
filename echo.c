#include "echo.h"
#include "headers.h"

void echo_echo(char* command)
{
	char output[strlen(command)+10];
	strcpy(output, command);
	int i = 0;
//	output[strcspn(output, "\n")] = 0; //interesting
	bool check_for_spaces = true;
	for(i = 0; output[i]; i++)	//space & tab handling
	{
		if(output[i]!=' ' && output[i]!='\t')
		{
			printf("%c", output[i]);
			check_for_spaces = false; //one space after every word is allowed
		}
			
		else if((output[i]==' ' || output[i]=='\t') && check_for_spaces==false)
		{
			printf(" ");
			check_for_spaces = true;
		}
	}
	printf("\n");
}
