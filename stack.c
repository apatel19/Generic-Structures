
/*
        @author aipatel1
        @stac.c is stack implemantation using doubly linked list implementation
        CWID: 11598639
*/


#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "stack.h"
#include"integer.h"
#include"dll.h"

struct Node {
        struct Node *next;
        struct Node *prev;
        void *value;
};

struct stack {
        struct Node *head;
        struct Node *tail;
        int size;
        void (*display) (void *, FILE *fp);
        void (*free) (void *v);
};

//Stack -> LIFO
STACK *newSTACK(void (*d)(void *,FILE *),void (*f)(void *)){
     STACK *items =  malloc(sizeof(STACK));
        assert(items != 0);
        items->head = NULL;
        items->tail = NULL;
        items->size = 0;
        items->display = d;
        items->free = f;
        return items;
}

static struct Node* Node_Create (void *value) {
        struct Node *node = malloc(sizeof(struct Node));
        if (node) {
        	node->value = value;
        	node->next = NULL;
        	node->prev = NULL;
        }
        return node;
}

void insertAtHead(STACK *items, void *value) {
    	struct Node *newNode = Node_Create(value);
    		if(items->head==NULL) {
        			//For now, newNode is the only one node of list 
        		items->head=newNode;
        		items->tail=newNode;
    		} else { 
    			newNode->next = items->head;
    			items->head->prev = newNode; //before, the previous node of head was NULL. but now newNode
    			items->head = newNode; //update the global node 'head' by newNode
    	}
}

void insertAtTail(STACK *items, void* value) {
   	struct Node *newNode = Node_Create(value);
    		if(items->head==NULL) {
        		items->head=newNode;
        		items->tail=newNode;
    		} else { 
    			items->tail->next = newNode;
    			newNode->prev = items->tail; //'newNode' point 'tail' node as previous node
    			items->tail = newNode; // update the global node 'tail' by newNode. 
     	}
}

void push(STACK *items,void *value) {
        insertAtHead(items, value);
        items->size++;
}

void *pop(STACK *items) {
	assert(sizeSTACK(items) > 0);
	void *p = NULL;  
        struct Node *current = NULL;	
		current = items->head->next;
                p = items->head->value;

                free(items->head);
                items->head = current;

                items->size--;
                return p;
}

void *peekSTACK(STACK *items){
	assert(sizeSTACK(items) > 0);
	if (items->head != NULL)
		return items->head->value;
	else 
		return NULL;
}

int sizeSTACK(STACK *items){
	return items->size;
}

void displaySTACK(STACK *items,FILE *fp){
	struct Node *temp = items->head;
 	void *lastElement = NULL;
 	printf ("|");
	if (items->head == NULL) {printf ("|");}
	else {	       
		 while (temp != NULL)
          	{
          		if (temp->next == NULL) {
          			lastElement = temp->value;
          			items->display(lastElement,fp);
          			break;
          		}
          		items->display(temp->value,fp);
          		printf (",");
          		temp = temp->next;
          	}
	  	printf("|");
	  }	
}

void displaySTACKdebug(STACK *items,FILE *fp){
	struct Node *t = items->head;
	void *lastElement = NULL;
        	if (items->head == NULL) {
        		printf ("head->{{}},tail->{{}}");
        	} else {
        		printf ("head->{{");
	  		while (t != NULL) {
          			if (t->next == NULL){
          				lastElement = t->value;
          				items->display(lastElement,fp);
          				break; 
          			}
          			items->display(t->value,fp);
          			printf (",");
          			t = t->next;
          	} 
        	printf("}},tail->{{"); 
        	items->display(lastElement,fp);
        	printf("}}");
        }	
}

void freeSTACK(STACK *items){
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
