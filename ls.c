#include "ls.h"
#include "headers.h"

void ls(const char* dir, bool aflag, bool lflag)
{
	struct dirent *d; //for reading files in the folder
	DIR* de = opendir(dir); //de is the folder we are looking into
	char error_message[strlen(dir)+20];
	
	// FOR PRINTING APPROPRIATE ERROR MESSAGE
	strcpy(error_message, "ls: cannot access '");
	strcat(error_message, dir);
	strcat(error_message, "'");
	
	if(!de)
	{
		if(errno==ENOENT)
		{
			perror(error_message);
		}
		else
			perror("Unable to find directory");
		return;
	}
	
	struct stat sb;
	struct passwd *file_owner;
	struct group *file_group;
	char filestat[PATH_MAX];
	
	if(dir!="." && (dir[0]!='.' && dir[1]!='\0'))
	{
		printf("%s:\n", dir);
	}
	
	if(aflag==false && lflag==false)
	{
		while((d=readdir(de))!=NULL)
		{
			if(d->d_name[0]!='.')
				printf("%s\n", d->d_name);
		}
		printf("\n");
	}
	
	else if(aflag==true && lflag==false)
	{
		while((d=readdir(de))!=NULL)
		{
			printf("%s\n", d->d_name);
		}
		printf("\n");
	}
	
	else if(aflag==false && lflag==true)
	{
		long long unsigned totalcount = 0;
		while((d=readdir(de))!=NULL)
		{
			if(d->d_name[0]!='.')
			{
				sprintf(filestat, "%s/%s", dir, d->d_name);
				stat(filestat, &sb);
				
				//filetype
		
		        switch (sb.st_mode & S_IFMT) 
				{
		           	case S_IFBLK:
				   		printf("b");	//block device     
						break;
		         	case S_IFCHR:  
				   		printf("c");	//character device
						break;
		            case S_IFDIR:  
				   		printf("d");	//directory         
						break;
		            case S_IFIFO:  
				   		printf("p");	//FIFO/pipe            
						break;
		            case S_IFLNK:  
				   		printf("l");	//symlink                
						break;
		            case S_IFREG:  
						printf("-");	//regular file         
						break;
		            case S_IFSOCK: 
						printf("s");	//socket                  
						break;
		            default:       
				   		printf("?");	//unknown            
						break;
		        }
		        
		        //permissions
		        
		        printf((sb.st_mode & S_IRUSR) ? "r" : "-");
				printf((sb.st_mode & S_IWUSR) ? "w" : "-");
				printf((sb.st_mode & S_IXUSR) ? "x" : "-");
				printf((sb.st_mode & S_IRGRP) ? "r" : "-");
				printf((sb.st_mode & S_IWGRP) ? "w" : "-");
				printf((sb.st_mode & S_IXGRP) ? "x" : "-");
				printf((sb.st_mode & S_IROTH) ? "r" : "-");
				printf((sb.st_mode & S_IWOTH) ? "w" : "-");
				printf((sb.st_mode & S_IXOTH) ? "x" : "-");
				
				printf(" ");
				
				//number of hardlinks
				
				printf("%d\t", sb.st_nlink);
				
				//file owner and file group
				
				file_owner = getpwuid(sb.st_uid);
				file_group = getgrgid(sb.st_gid);
				
				printf("%s\t%s\t", file_owner->pw_name, file_group->gr_name);
				
				//size in bytes
				
				printf("%llu\t",sb.st_size);
				
				totalcount += sb.st_size;
				
				//time of last modification & filename
				
				char last_mod[21];
				strftime(last_mod, 21, "%b %d %Y %H:%M", localtime(&(sb.st_mtime)));

				printf("\t%s %s\n", last_mod, d->d_name);
				
//				//filename
//				
//				printf("%s\n", d->d_name);
			}
		}
		printf("total %llu\n", totalcount/512);
	}
	
	else //both true
	{
		long long unsigned totalcount = 0;
		while((d=readdir(de))!=NULL)
		{
			sprintf(filestat, "%s/%s", dir, d->d_name);
			stat(filestat, &sb);
				
			//filetype
		
		    switch (sb.st_mode & S_IFMT) 
			{
		        case S_IFBLK:
				   	printf("b");	//block device     
					break;
				case S_IFCHR:  
				   	printf("c");	//character device
					break;
		    	case S_IFDIR:  
				   	printf("d");	//directory         
					break;
		        case S_IFIFO:  
				   	printf("p");	//FIFO/pipe            
					break;
		        case S_IFLNK:  
				   	printf("l");	//symlink                
					break;
		        case S_IFREG:  
					printf("-");	//regular file         
					break;
		        case S_IFSOCK: 
					printf("s");	//socket                  
					break;
		        default:       
				   	printf("?");	//unknown            
					break;
		    }
		        
		    //permissions
		        
		    printf((sb.st_mode & S_IRUSR) ? "r" : "-");
			printf((sb.st_mode & S_IWUSR) ? "w" : "-");
			printf((sb.st_mode & S_IXUSR) ? "x" : "-");
			printf((sb.st_mode & S_IRGRP) ? "r" : "-");
			printf((sb.st_mode & S_IWGRP) ? "w" : "-");
			printf((sb.st_mode & S_IXGRP) ? "x" : "-");
			printf((sb.st_mode & S_IROTH) ? "r" : "-");
			printf((sb.st_mode & S_IWOTH) ? "w" : "-");
			printf((sb.st_mode & S_IXOTH) ? "x" : "-");
				
			printf(" ");
				
			//number of hardlinks
				
			printf("%d\t", sb.st_nlink);
				
			//file owner and file group
				
			file_owner = getpwuid(sb.st_uid);
			file_group = getgrgid(sb.st_gid);
			
			printf("%s\t%s\t", file_owner->pw_name, file_group->gr_name);
				
			//size in bytes
				
			printf("%llu\t",sb.st_size);
			
			totalcount += sb.st_size;
				
			//time of last modification & filename
			char last_mod[21];
			strftime(last_mod, 21, "%b %d %Y %H:%M", localtime(&(sb.st_mtime)));

			printf("\t%s %s\n", last_mod, d->d_name);
				
//			//filename
//				
//			printf("%s\n", d->d_name);
			
		}
		printf("total %llu\n", totalcount/512);
	}
	
	closedir(de);
}

void list_dir(char* command)
{
	if(command=="\0")
	{
		ls(".", false, false);
		return;
	}
	int i = 0;
	bool empty = true;
	bool aflag = false;
	bool lflag = false;
	int count = 1;
	int sz = strlen(command);
	
	for(i = 0; i<sz; i++)
	{
		if(command[i]!=' ' && command[i]!='\t')
		{
			empty = false;
			if(command[i]=='-')
			{
				if(command[i+1]=='a')
				{
					aflag = true;
					if(command[i+2]=='l')
						lflag = true;
				}
					
				else if(command[i+1]=='l')
				{
					lflag = true;
					if(command[i+2]=='a')
						aflag = true;
				}
					
				else
				{
					perror("Invalid Flag");
					return;
				}
			}
		}
		else
		{
			count++; //NUMBER OF OPTIONS ENTERED >= NUMBER OF DIRECTORIES
		}
	}
	
	if(empty==true)
	{
		ls(".", false, false);
	}
	
	else
	{
		char* directories[count];
		
		int j = 0, k = 0;
		char* temp = malloc(sz*sizeof(char));
		for(i = 0;i<sz; i++)
		{
			if(command[i]!=' ' && command[i]!='\t')
			{
				temp[j++] = command[i];
			}
			else
			{
				temp[j] = '\0';
				j = 0;
				directories[k] = malloc(strlen(temp)*sizeof(char));
				strcpy(directories[k], temp);
				k++;
				temp[0] = '\0';
			}
		}
		
		if(j!=0)
		{
			temp[j] = '\0';
			j = 0;
			directories[k] = malloc(strlen(temp)*sizeof(char));
			strcpy(directories[k], temp);
			k++;
			temp[0] = '\0';
		}
		
		free(temp);
		temp = NULL;
		
		bool no_dir = true;
		for(i = 0;i<k;i++)
		{
			if(!strcmp(directories[i], "\0") || !strcmp(directories[i], "-a") || !strcmp(directories[i], "-l") || !strcmp(directories[i], "-al") || !strcmp(directories[i], "-la"))
			{
				//do nothing
			}
			else
			{
				ls(directories[i], aflag, lflag);
				no_dir = false;
			}	
			free(directories[i]);
			directories[i] = NULL;
		}
		if(no_dir==true) // IF NO DIRECTORIES ARE DETECTED, PRINT FOR PWD
			ls(".", aflag, lflag);
	}
}
