
/*
        @author aipatel1
        @queue.c is signly linked list implementation
        CWID: 11598639
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "queue.h"
#include "sll.h"

struct Node {
	struct Node *next;
	void *value;
};

struct queue {
	struct Node *head;
	struct Node *tail;
	int size;
	void (*display) (void *, FILE *fp);
        void (*free) (void *v);
};


QUEUE *newQUEUE(void(*d)(void *, FILE*), void (*f)(void *))
	{
	QUEUE *items = malloc(sizeof(QUEUE));	
	assert(items != 0);
        items->head = NULL;
        items->tail = NULL;
        items->size = 0;
        items->display = d;
        items->free = f;
        return items;
	}

static struct Node *Node_Create(void *value)
        {
        struct Node *node = malloc(sizeof(struct Node));
	        if (node) {
        		node->value = value;
        		node->next = NULL;
        	}
        	return node;
}

void insert_At_Head(QUEUE *items, void *value)
        {
        struct Node *newNode = Node_Create(value);
	        if(items->head==NULL) //For the 1st element
        	{ 
        		items->head=newNode;
        		items->tail=newNode;
        	} else{  
        		newNode->next = items->head;
        		items->head = newNode; 
        	}
}

void insert_At_Tail (QUEUE *items, void *value)
        {
        struct Node *newNode = Node_Create(value);
        	if(items->head==NULL) //For the 1st element
        	{
        		items->head=newNode;
        		items->tail=newNode;
        	} else {
        		items->tail->next = newNode; 
        		items->tail = newNode; // update the global node 'tail' by newNode. 
     		}
}


void enqueue(QUEUE *items,void *value)
	{
        if (items->head == NULL){
        	insert_At_Head(items,value);
		items->size++;
        } else {
        	insert_At_Tail(items,value);
		items->size++;
        }
}


void *dequeue(QUEUE *items)
	{
	assert(sizeQUEUE(items)>0);
	void *p = NULL;  
        struct Node *current = NULL;  
                current = items->head->next;
                p = items->head->value;
                free(items->head);
                items->head = current;
                items->size--;
	        return p;
	}

void *peekQUEUE(QUEUE *items)
	{	
	assert(sizeQUEUE(items)>0);
	if (items->head != NULL)
        return items->head->value;
        else return NULL;
	}


int sizeQUEUE(QUEUE *items)
	{
	return items->size; 
	}


void displayQUEUE(QUEUE *items,FILE *fp)
{
	struct Node *temp = items->head;
	void *lastElement = NULL;
 	printf ("<");
	while(temp != NULL){
        	if (temp->next == NULL)
        	{
        	lastElement = temp->value;
        	items->display(lastElement,fp);
        	break;
        	}
        	items->display(temp->value,fp);
        	printf (",");
        	temp = temp->next;
        }
	printf(">");
}

void displayQUEUEdebug(QUEUE *items,FILE *fp)
{	
	struct Node *t = items->head;
	void *lastElement = NULL;
        if (t == NULL)
        	{
        	printf ("head->{},tail->{}");
        	}
        else {
        	printf ("head->{"); 
        	while(t != NULL){
        		if (t->next == NULL)
        		{
        			lastElement = t->value;
        			items->display(lastElement,fp);
        			break;
        		}
        			items->display(t->value,fp);
        			printf (",");
        			t = t->next;
        		}
			printf("},tail->{");
        		items->display(items->tail->value,fp);
        		printf("}");
        }
}

void freeQUEUE(QUEUE *items)
	{
 struct Node *temp = items->head;
        while (temp != NULL) {
                struct Node *current = temp;
                temp = temp->next;
                if (items->free != NULL){
                items->free(current->value);
                items->size--;
		}
                free(current);
        }
free(items);
     } 
