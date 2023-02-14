#include "headers.h"
#include "jobs.h"

int comparator(const void *p, const void *q)
{
	struct job *a = (struct job *)p;
	struct job *b = (struct job *)q;
	int i = 0;
	while(i!=1024)
	{
		if(a->command[i] > b->command[i])
		{
			return 1;
		}
		else if(a->command[i] < b->command[i])
		{
			return -1;
		}
		i++;
	}
	return 0;
}

int newjobid()
{
	int max = 0;
	int i = 0;
	for(i = 0;i<64;i++)
	{
		if(list[i].jid!=-1)
		{
			if(max<list[i].jid)
				max = list[i].jid;
		}
	}
	return max+1;
}

void addjob(int bgpid, char* commandname)
{
	int i = 0;
	for(i = 0;i<64;i++)
	{
		if(list[i].pid==-1)
		{
			list[i].pid = bgpid;
			strcpy(list[i].command, commandname);
			list[i].jid = newjobid();
//			list[i].cmd_state = nothing here, for now. WAIT DID I EVEN NEED THIS?
			break;
		}
	}
//	printf("%d\n",list[i].pid);
}

void displayjobs(char *flags)
{
	qsort((void*)list, 64, sizeof(struct job), comparator);	//sort in alphabetical order of command name
	
	bool rflag = false;
	bool sflag = false;
	int len = strlen(flags);
	int i = 0;
	for(i = 0;i<len;i++)
	{
		if(flags[i]=='-')
		{
			if(flags[i+1]=='r')
			{
				rflag = true;
				if(flags[i+2]=='s')
					sflag = true;
				
			}
			else if(flags[i+1]=='s')
			{
				sflag = true;
				if(flags[i+2]=='r')
					rflag = true;
			}
		}
		if(sflag==true && rflag==true)
		{
			break;
		}
	}
	
	for(i = 0;i<64;i++)
	{
		if(list[i].pid!=-1)
		{
			char path1[1024];
			sprintf(path1, "/proc/%d/stat", list[i].pid); //PID(1), STATE(3) memory(23) 
			FILE* fptr1 = fopen(path1, "r");
			if(fptr1==NULL)
			{
				// Since we already know this, I'm going to use this to remove things from the list
				list[i].pid = -1;
				list[i].jid = -1;
				strcpy(list[i].command, "\0");
				strcpy(list[i].command, "\0");
				
//				perror("Error opening /proc/pid/stat file");
			}
			else
			{
				char ch;
				int ii = 0;
				do
				{				
					ch = fgetc(fptr1);
					if(ch==' ')
					{
						ii++;
						if(ii==2)	//get process state
						{
							ch = fgetc(fptr1);
							break;
						}
					}
					
				}while(ch != EOF);
				if(ch=='R' || ch=='S' || ch=='D') //D is uninterruptible sleep
				{
					if(rflag==false && sflag==true)
					{
						continue;
					}
				}
				else
				{
					if(rflag==true && sflag==false)
					{
						continue;
					}
				}
				fclose(fptr1);
				printf("[%d] ", list[i].jid);
				if(ch=='R' || ch=='S' || ch=='D')
				{
					printf("Running");
				}
				else
				{
					printf("Stopped");
				}
				printf(" %s [%d]\n", list[i].command, list[i].pid);
			}
		}
	}	
}


