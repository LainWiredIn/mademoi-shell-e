#include<stdlib.h>
#include<stdio.h>
#include<sys/utsname.h>
#include<limits.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<dirent.h> //for implementing ls command
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<sys/wait.h>
#include<errno.h>
#include<time.h>
#include<fcntl.h>
#include<signal.h>
#include "jobs.h"

#define QSIZE 20

char* home_dir;
char* prev_dir; //for implementation of cd -

int in_len;

// for history command
char* hqueue[QSIZE];

int front;
int rear;

struct job list[64]; //for job handling
struct job fgjob;	//for keeping track of current foreground job



