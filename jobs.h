#ifndef __JOBS_H
#define __JOBS_H

struct job
{
	pid_t pid;	//process id
	int jid;	//job id
	char command[1024];	//command name
	char cmd_state[1024]; //command state - can either be running or stopped
};

int comparator(const void *p, const void *q);
int newjobid();
void addjob(int bgpid, char* commandname);
void displayjobs(char *flags);

#endif
