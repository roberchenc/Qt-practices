#pragma once
#include <winsock2.h>

#define SUCCESS             1
#define PARAMETER_ERROR    -1
#define SYSTEM_ERROR       -2
 
typedef struct node
{
	int length;
	int read_count;
	char data[1024];
	struct node *next;
}*Queue;

//int data_copy(char *dst, const char *src, int length);

int queue_isEmpty(Queue queue);

int queue_Create(Queue *queue);

void queue_Destory(Queue *queue);

int queue_Front(Queue queue, char *buff, int length);

int queue_Write(Queue queue, const char *buff, int length);

int queue_Read(Queue queue, char *buff, int length);
