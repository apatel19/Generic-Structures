/*
	@author aipatel1
	@sll.c is signly linked list implementation
	CWID: 11598639
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "sll.h"
#include "integer.h"
struct Node {
        struct Node *next;
        void *value;
};

struct sll {
        struct Node *head;
        struct Node *tail;
        int size;
        void (*display) (void *, FILE *fp);
        void (*free) (void *v);

};

//Creates an empty SLL node
//@param1 is displayInteger Function
//@param2 is freeINTEGER function
//return newNode*
SLL *newSLL(void (*d)(void *v,FILE *fp),void (*f)(void *v)) {
	SLL *items = malloc(sizeof(SLL));
        assert(items != 0);
        items->head = NULL;
        items->tail = NULL;
        items->size = 0;
        items->display = d;
        items->free = f;
        return items;
}

//@param void value
//return newlly created node pointing to NULL
static struct Node *Node_Create(void *value)
	{
	struct Node *node = malloc(sizeof(struct Node));
	if (node) { 
	node->value = value;
	node->next = NULL;
	}
	return node;
	}

//@param1 = pointer to node
//@param2 = generic value pointer
//funtion inset node at head of emply list
void insertAtHead(SLL *items, void *value)
	{
    	struct Node *newNode = Node_Create(value);
    	if(items->head==NULL) //For the 1st element
    	{   
        //For now, newNode is the only one node of list 
        items->head=newNode;
        items->tail=newNode;
    	}
     	else {
    	//newNode will be the NEW HEAD of list.
    	//So it'll point to head as 'next node'
    	newNode->next = items->head; 
    	items->head = newNode; //update the global node 'head' by newNode
	}
	}

//@param1 = pointer to node
//@param2 = generic value pointer
//funtion inset node at tail of list
void insertAtTail (SLL *items, void *value)
	{
	struct Node *newNode = Node_Create(value);
	if(items->head==NULL) //For the 1st element
    	{   
        items->head=newNode;
        items->tail=newNode;
   	}
        else { 
    	//'tail' is a global node. 'newNode' will be the next node of tail.
     	//finally newNode will be the 'tail' node of the list
    	items->tail->next = newNode; 
    	//newNode->prev = items->tail; //'newNode' point 'tail' node as previous node
    	items->tail = newNode; // update the global node 'tail' by newNode. 
	}
	}

void insertSLL(SLL *items,int index,void *value) {
	if (index == 0 || items->head == NULL) {
		insertAtHead(items,value);
		items->size++;
	}
	else if (index == sizeSLL(items)){
		insertAtTail(items,value);
        	items->size++;
	}
	else {	//Insert node at middle index
		struct	Node *temp = items->head;
		for (int i = 0; i < index-1; i++)
                {	
			temp = temp->next;
                }
		struct Node *newNode = Node_Create(value);
		struct Node *headOfnextNode = temp->next;	 	
		temp->next = newNode;
		newNode->next = headOfnextNode;
		items->size++;
	}	
}
 
//@param - list
//return - size (int) of list
int sizeSLL(SLL *items){
	if (items != NULL)
 	return items->size;

	else return 0;
}


//@param1 list
//@param2 FILE* which prints to stdout
//function prints value from head to tail order
void displaySLL(SLL *items,FILE *fp) {
	struct Node *temp = items->head;//malloc(sizeof(SLL));
	void *lastElement = NULL;
 	printf ("{");
	while(temp != NULL) {
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
	printf("}");   
}

void *removeSLL(SLL *items, int index)
        {	        
	void *result = NULL;
	struct Node *current = NULL;
        if (index == 0)
        {
                current = items->head->next;
                result = items->head->value;		
                items->free(items->head);
		items->head = current;
		items->size--;
       	}
        else {
		struct Node *temp = items->head;
                int i;
                for(i=0; i< index-1; i++) {
                	temp = temp->next;
                }        

		if(temp != NULL)
                {
                	result = temp->next->value;     
			if (temp->next->next == NULL)
			{
				struct Node *toBeDeleted = temp->next;
				temp->next = NULL;		
				items->tail = temp;	
				items->free(toBeDeleted);
			}
			else {
				struct Node *toBeDeleted = temp->next;	
				temp->next = temp->next->next;	
				free(toBeDeleted);
			}
        		items->size--;
		}
	}
	
	return result;	
}


void unionSLL (SLL *recipient, SLL *donor)
{

	  if (donor != NULL && sizeSLL(donor)  > 0 && donor->head != NULL && recipient->head != NULL)
        {
                int s_rec = sizeSLL(recipient);
                int s_don = sizeSLL(donor);
                int s_total = s_rec + s_don;
                recipient->tail->next = donor->head;
                recipient->tail = donor->tail;
                recipient->size = s_total;
                recipient->tail->next = NULL;
                donor->head = NULL;
                donor->size = 0;
        }
        else if (recipient->head == NULL && donor->head != NULL)
        {
                int s_rec = sizeSLL(recipient);
                int s_don = sizeSLL(donor);
                int s_total = s_rec + s_don;
                recipient->head = donor->head;
                recipient->tail = donor->tail;
                recipient->size = s_total;
                recipient->tail->next = NULL;
                donor->head = NULL;
                donor->size = 0;
        }
 
}

void displaySLLdebug(SLL *items,FILE *fp){
 	void *lastElement = NULL;
 	struct Node *temp = items->head;
        if (temp == NULL)
        {
        	printf ("head->{},tail->{}");
        }
        else {
        	printf ("head->{");
		while(temp != NULL) {
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
		printf("},tail->{");
        	items->display(lastElement,fp);
        	printf("}");
        } 
}

void *getSLL(SLL *items,int index)
        {        
	if (index == sizeSLL(items))
        index = index - 1;        
	struct Node *hold = items->head;
	void *found = NULL;
	int i = 0;
        if(index == 0) {
        found = hold->value;
        }
        else if (index == sizeSLL(items)-1){
        found = items->tail->value;
        }
        else {
        while (hold != NULL)
                {
                if (i == index)
                  {
                  found = hold->value;
                  break;
                  }
                hold = hold->next;
                i++;
                }
        }
return found;
	}

void *setSLL(SLL *items,int index,void *value)
        {        
	struct Node *set = items->head;
	void *prevValue = NULL;
	int i = 0;
        if (sizeSLL(items) == index)
                { 
                insertSLL(items, index, value); 
		return prevValue;
                }

	if(index == 0) {
                prevValue = set->value;
                set->value = value; 
        }
        else if (index == sizeSLL(items)-1){
                prevValue = items->tail->value;
                items->tail->value = value;
        }
	else {
	while (set != NULL)
                {
                        if (i == index)
                        {
                                prevValue = set->value;
                                set->value = value;
                                break;
                        }
                                set = set->next;
                                i++;
                 }
	}	
  	return prevValue;
	}

void freeSLL(SLL *items)
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


