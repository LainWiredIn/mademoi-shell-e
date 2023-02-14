#include "history.h"
#include "headers.h"

int isFull()
{
	if((front == rear+1) || (front == 0 && rear == QSIZE - 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isEmpty()
{
	if(front==-1)
		return 1;
	else
		return 0;
}

void dequeue()
{
	if(isEmpty())
	{
		return;
	}
	else
	{
		if(front==rear)
		{
			front = -1;
			rear = -1;
		}
		else
		{
			front = (front+1)%QSIZE;
		}
		return;
	}
}

void display(int num)
{
	int i = 0;
	if(isEmpty())
	{
		printf("No history recorded\n");
	}
	else
	{
		if(num==-1)
		{
			if(rear<10)
			{
				for(i = 0; i<10; i++)
				{
					if(hqueue[i]==NULL)
					{
						break;
					}
					printf("%d %s\n", i+1, hqueue[i]);
				}
			}
			else
			{
				for(i = rear - 9; i<=rear; i++)
				{
					if(hqueue[i]==NULL)
					{
						break;
					}
					printf("%d %s\n", i+1, hqueue[i]);
				}
			}
		}
		else
		{
			for(i = rear - num + 1; i<=rear; i++)
			{
				if(hqueue[i]==NULL)
				{
					break;
				}
				printf("%d %s\n", i+1, hqueue[i]);
			}
		}
		
	}
}

void enqueue(char* element)
{
	if(isFull())
	{
		dequeue();
	}
	if(front == -1)
		front = 0;
	int prev = rear;
	rear = (rear + 1)%QSIZE;

	if(hqueue[rear-1]!=NULL && rear!=0 && !strcmp(hqueue[rear-1], element))
	{
		rear = prev;
	}
	else if(hqueue[QSIZE-1]!=NULL && rear==0 && !strcmp(hqueue[QSIZE-1], element))
	{
		rear = prev;
	}
	else
	{
		hqueue[rear] = (char*)malloc(strlen(element)*sizeof(char));
		strcpy(hqueue[rear], element);
	}
}


