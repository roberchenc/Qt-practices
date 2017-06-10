#include <string.h>
#include <stdlib.h>
#include "myQueue.h"


int queue_isEmpty(Queue queue)
{
	if (queue == NULL)
		return 0;
	if (queue->length == 0)
		return 1;
	return 0;
}

int queue_Create(Queue *queue)
{
	*queue = (node *)malloc(sizeof(node));
	if (queue == NULL)
		return SYSTEM_ERROR;

	(*queue)->length = 0;
	(*queue)->read_count = 0;
	(*queue)->next = NULL;
	return SUCCESS;
}

void queue_Destory(Queue *queue)
{
	if (queue == NULL || *queue == NULL)
		return;
	node *p, *q;
	p = (*queue)->next;
	while (p != NULL)
	{
		q = p;
		p = p->next;
		//free(q->data);
		//q->data = NULL;
		free(q);
		q = NULL;
	}
	free((*queue));
	((*queue)) = NULL;
}

int queue_Front(Queue queue, char *buff, int length)
{
	if (queue == NULL || buff == NULL || queue->length < length)
		return PARAMETER_ERROR;
	int count = 0;
	node *p = queue;
	while (count < length)
	{
		p = p->next;
		if (p == NULL)
			return PARAMETER_ERROR;
		if (length - count >= p->length - p->read_count)
		{
			memcpy(buff, p->data + p->read_count, length - count);
			count += (p->length - p->read_count);
		}
		else
		{
			memcpy(buff, p->data + p->read_count, length - count);
			//p->read_count += length - count;
			count += (length - count);
			return count;
		}
	}
	return count;
}

int queue_Write(Queue queue, const char *buff, int length)
{
	if (queue == NULL || buff == NULL || length <= 0)
		return PARAMETER_ERROR;
	node *p = queue;
	while (p->next != NULL)
		p = p->next;

	p->next = (node *)malloc(sizeof(node));
	if (p->next == NULL)
		return SYSTEM_ERROR;
	p = p->next;
	p->length = length;
	p->read_count = 0;
	p->next = NULL;
	//p->data = (char *)malloc(sizeof(char)*length);
	//if (p->data == NULL)
		//return SYSTEM_ERROR;
	memcpy(p->data, buff, length);

	queue->length += length;

	return SUCCESS;
}

int queue_Read(Queue queue, char *buff, int length)
{
	if (queue == NULL || buff == NULL || queue->length < length)
		return PARAMETER_ERROR;
	int count = 0;
	node *p;
	while (count < length)
	{
		p = queue->next;
		if (length - count >= p->length - p->read_count)
		{
			memcpy(buff, p->data + p->read_count, length - count);
			count += (p->length - p->read_count);
			queue->length -= (p->length - p->read_count);
			queue->next = queue->next->next;

			//free(p->data);
			//p->data = NULL;
			free(p);
			p = NULL;
		}
		else
		{
			memcpy(buff, p->data + p->read_count, length - count);
			p->read_count += length - count;

			queue->length -= (length - count);
			count += (length - count);
			return count;
		}
	}

	return count;
}
