
/*
        @author aipatel1
        @dll.c is doubly linked list implementation
        CWID: 11598639
*/


#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<assert.h>
#include "dll.h"
#include"integer.h"

struct Node {
        struct Node *next;
        struct Node *prev;
	void *value;
};

struct dll {
        struct Node *head;
        struct Node *tail;
        int size;
        void (*display) (void *, FILE *fp);
        void (*free) (void *v);

};

DLL *newDLL(void (*d)(void *,FILE *),void (*f)(void *)){
	DLL *items = malloc(sizeof(DLL));
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

void insert_At_Head(DLL *items, void *value)
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
    items->head->prev = newNode; //before, the previous node of head was NULL. but now newNode

    items->head = newNode; //update the global node 'head' by newNode
        }
}

void insert_At_Tail(DLL *items, void* value)
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
    newNode->prev = items->tail; //'newNode' point 'tail' node as previous node
    items->tail = newNode; // update the global node 'tail' by newNode. 
     }
}

void insert_At_Middle(DLL *items, void *value, int position)
{
    struct Node *newNode = Node_Create(value); 
    if(items->head==NULL) {//For the 1st element
        //For now, newNode is the only one node of list
        //So it it is head and also tail
        items->head=newNode;
        items->tail=newNode;
        return;
    }
    struct Node *temp = items->head;
    int i = 0;
    //find the position where our newNode will put
	if (position <= sizeDLL(items)/2){  //Find in first half
	while (i < position-1) {	
		temp = temp->next;
		i++;
	 	}
	newNode->next = temp->next;
	newNode->prev = temp;
	
	if(temp->next)
        temp->next->prev = newNode; //newNode will be the previous node of temp->next node

    	temp->next = newNode;
	} else if (position >= sizeDLL(items)/2){ //Find in second half
		temp = items->tail;
		i = sizeDLL(items);
		while (temp != NULL){
		if (i == position) {break;}
		
		temp = temp->prev;
		i--;
		}
		newNode->next = temp->next;
		newNode->prev = temp;

	if (temp->next)
	temp->next->prev = newNode;
	
	temp->next = newNode;
	} else {
	while (temp != NULL) {
		if (i == position) {break;}
		temp = temp->next;
		i++;
		}
		newNode->next = temp->next;
        	newNode->prev = temp;

        if(temp->next)
        temp->next->prev = newNode; //newNode will be the previous node of temp->next node

        temp->next = newNode;
	}
}


void insertDLL(DLL *items,int index,void *value){
	assert(index >= 0 && index <= sizeDLL(items));
	if (index == 0 || items->head == NULL) {
	insert_At_Head(items, value);
	items->size++;
	} else if (index == sizeDLL(items)) {	
	insert_At_Tail(items,value);
	items->size++;
	} else {
	insert_At_Middle(items,value,index);
	items->size++;
	}
}

void *removeDLL(DLL *items,int index){
	assert(sizeDLL(items) > 0 && index >= 0 && index < sizeDLL(items));
	void *p = NULL;
        if (index == sizeDLL(items) && index != 0){ index = index - 1; }
	struct Node *current = NULL;
	if (index == 0) {	
	 	current = items->head->next;
	 	p = items->head->value;
         
	 	free(items->head);
	 	items->head = current;
	
	 	items->size--;
	 	return p;
	} else if (index == sizeDLL(items)-1) {	 
    	    	if(items->tail == NULL)	{	
	 	} else{
		p = items->tail->value;
        	current = items->tail;
        	items->tail = items->tail->prev; // Move last pointer to 2nd last node
        	items->tail->next = NULL; // Remove link to of 2nd last node with last node
        	free(current);       // Delete the last node
        	items->size--; 	
		} 
		return p;
	} else if (index <= (sizeDLL(items)/2)) { 
	struct Node *holder = items->head;
        int i = 0;
	while (holder != NULL){
	if (i == index)	{break;}
	holder = holder->next;
	i++;
	}
	p = holder->value;
        holder->prev->next = holder->next;
        holder->next->prev = holder->prev;

        free(holder); // Delete the n node
        items->size--; 
	return p;
	}
	else if (index >= (sizeDLL(items)/2)){	
	struct Node *holder = items->tail;
	int i = sizeDLL(items)-1;
	
	while (holder != NULL){
	if (i == index) {break;}
	i--;
	holder = holder -> prev;
	}
	p = holder->value;
	holder->prev->next = holder->next;
        holder->next->prev = holder->prev;

        free(holder); // Delete the n node
        items->size--; 
	return p;
	} else {
                struct Node *holder = items->head;
                int i;
                for(i=1; i<=index && holder!=NULL; i++){
                        holder = holder->next;
                }
                        if(holder != NULL) {
                                p = holder->value;
                                holder->prev->next = holder->next;
                                holder->next->prev = holder->prev;
                                free(holder); // Delete the n node
                                items->size--; 
                        }
                        return p;
           }
}

void unionDLL(DLL *recipient,DLL *donor){

	 if (donor != NULL && sizeDLL(donor)  > 0 && donor->head != NULL && recipient->head != NULL) 
        {
                int s_rec = sizeDLL(recipient);
                int s_don = sizeDLL(donor);
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
		int s_rec = sizeDLL(recipient);
                int s_don = sizeDLL(donor);
                int s_total = s_rec + s_don;
                recipient->head = donor->head;
                recipient->tail = donor->tail;
                recipient->size = s_total;
                recipient->tail->next = NULL;
                donor->head = NULL;
                donor->size = 0;
	}			 
}

void *getDLL(DLL *items,int index){
assert(index >= 0 && index < sizeDLL(items));
struct Node *hold = items->head;
void *found = NULL;
int i = 0;
	if(index == 0) {
	found = items->head->value;
	} else if (index <= (sizeDLL(items))/2){	
		while (hold != NULL) {
		  	if (i == index){
		  		found = hold->value;
		  		break;
		  	}
		 	hold = hold->next;
		 	i++;
		}
 	} else if (index >= (sizeDLL(items))/2){
		struct Node *goFromBack	= items->tail;
		i = sizeDLL(items)-1;
		while (goFromBack != NULL) {
	  		if (i == index) {
	  			found = goFromBack->value; 
          			break;		
	  		}
	  		i--;
          		goFromBack = goFromBack->prev;	  
	  	}
	} else if (index == sizeDLL(items)-1){
		found = items->tail->value;
	} else {
		while (hold != NULL) {
			if (i == index) {
                  		found = hold->value;
                  		break;
                  	}
			hold = hold->next;
			i++;
		}
	}
return found;
}

void *setDLL(DLL *items,int index,void *value){
assert(index >= 0 && index <= sizeDLL(items));
struct Node *set = items->head;
void *prevValue = NULL;
int i = 0;
	
	if (sizeDLL(items) == index) {	
                insertDLL(items, index, value);
                return prevValue;
		}
        if(index == 0) {
        	prevValue = set->value;
		set->value = value;
        } else if (index <= (sizeDLL(items))/2){ 
                while (set != NULL) {
                  	if (i == index) {
                  		prevValue = set->value;
		  		set->value = value;
                  		break;
                  	}
                  	set = set->next;
                 	i++;
                }
        } else if (index >= (sizeDLL(items))/2){
		i = sizeDLL(items)-1;
		set = items->tail;
        	while (set != NULL) {
          		if (i == index) {
          			prevValue = set->value;
          			set->value = value;
	  			break;
          		}
          		i--;
          		set = set->prev;
          	}
        } else if (index == sizeDLL(items)-1){
        	prevValue = items->tail->value;
        	items->tail->value = value;
	} else {
        	while (set != NULL) {
                	if (i == index) {
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

int sizeDLL(DLL *items){
return items->size;
}

void displayDLL(DLL *items,FILE *fp){
struct Node *temp = items->head;
void *lastElement = NULL;
printf ("{{");
	while(temp != NULL){
		if (temp->next == NULL){
			lastElement = temp->value;
			items->display(lastElement,fp);
			break;
		}
		items->display(temp->value,fp);
		printf (",");
        	temp = temp->next; 
	}
	printf("}}");
}

void displayDLLdebug(DLL *items,FILE *fp){
struct Node *temp = items->head;
	if (temp == NULL){
        	printf ("head->{{}},tail->{{}}");
        } else {
		printf ("head->");
		displayDLL(items, fp);
		printf(",tail->{{");
		items->display(items->tail->value,fp);
		printf("}}");
	}
}


void freeDLL(DLL *items){
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


