#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heap.h"
#include "bst.h"
#include <assert.h>
#include "queue.h"
#include "stack.h"
struct bstnode {
	void *value;
	BSTNODE *left;
	BSTNODE *right;
	BSTNODE *parent;
};

struct heap {
    BSTNODE *root;
    int size;
    void (*display)(void *, FILE *);
    int (*comparator)(void *, void *);
    void (*free) (void *v);
};


HEAP *newHEAP(
        void (*d)(void *v,FILE *fp),    //display
        int (*c)(void *v,void *f),     //compare
        void (*f)(void *v)) {
	HEAP *newHeap = (HEAP *)malloc(sizeof(HEAP));
	assert(newHeap != 0);
	newHeap->root = 0;
	newHeap->size = 0;
	newHeap->display = d;
	newHeap->comparator = c;
	newHeap->free = f;
	return newHeap;
}

BSTNODE *current = NULL;
QUEUE *q = NULL;
STACK *stack = NULL;
void insertHEAP(HEAP *h, void *value){
	if (h -> root != NULL){
	BSTNODE *node = newBSTNODE(value);
	
	if (current->left != NULL && current->right != NULL)
	{
		BSTNODE *temp = dequeue(q);//returnToph(q);
		push(stack, temp);
		current = temp;
		
	}
	if (current->left == NULL)
	{
		current->left = node;
		node->parent = current;
		enqueue(q, node);
		h->size++; 
	}
	else if (current->right == NULL)
	{
		current->right = node;
		node->parent = current;
		enqueue(q,node);
		h->size++; 
	}	
	} 
	else {
        h->root = newBSTNODE(value);
        h->size++;
	q = newQUEUE(0, 0);
	stack = newSTACK(0, 0);
	current = h->root;
	push(stack, h->root);
	} 
}
int  sizeHEAP(HEAP *h){
	return h->size;
}


/*void ShiftUp(HEAP* h, BSTNODE *node) {
     if(!node) return; 
     BSTNODE *largest = node;
	  //find the largest node
     if (node->left != NULL && h->comparator (largest->value, node->left->value) > 0) { 
        largest = node->left; 
     }
     if (node->right != NULL && h->comparator (largest->value, node->right->value) > 0) {
        largest = node->right;
     }
	 // if root has the largest value, return
     if (largest == node) return;
	 //if not, swap the node's value with largest value
     void* tmp = node->value;
     node->value = largest->value;
     largest->value = tmp;    
     ShiftUp(h,largest);
}

void HeapifyBinaryTree(HEAP* h, BSTNODE *root) {
    if (!root) return;
    if (root->left) {
        HeapifyBinaryTree(h,root->left); 
    }    
    if (root->right) {
        HeapifyBinaryTree(h,root->right);
    }
    ShiftUp(h,root);
}
*/
void heapify(BSTNODE *treeNode, HEAP *h){
BSTNODE *largest = NULL;
BSTNODE *le = treeNode->left;
BSTNODE *ri = treeNode->right;

    if(le != NULL && h->comparator (le->value, treeNode->value) < 0)
    {
        largest = le;
    }
    else
    {
        largest = treeNode;
    }

    if(ri != NULL && h->comparator (ri->value, largest->value) < 0)
    {
        largest = ri;
    }
    if(largest != treeNode)
    {
        void* temp = treeNode->value;
        treeNode->value = largest->value;
        largest->value = temp;
        heapify(largest, h);
    }
}
BSTNODE *last = NULL;

void buildHEAP(HEAP *h){

if (h->root == NULL){
printf ("Root nULL\n");
    return;
}
while (sizeQUEUE(q) != 0){
	BSTNODE *temp = dequeue(q);//returnToph(q);
	push(stack, temp);
	}

while (sizeSTACK(stack) != 0)
	{
	BSTNODE *temp = pop(stack);
	enqueue(q, temp);
	heapify(temp, h);
	}	
}

void *peekHEAP(HEAP *h){
  return h->root->value;
}

void *getNODE(BSTNODE *n) {
    return n->value;
}

void setNODE(BSTNODE *n, void *value) {
    n->value = value;
}
void *extractHEAP(HEAP *h) {
	if (h->root == NULL) return NULL;
	void *value = NULL;
	value = h->root->value;//getINTEGER(root->value);
        BSTNODE *last = dequeue(q);
        h->root->value = last->value;
        if(last->parent != NULL)
        {
            if(last->parent->left == last)
            {
                last->parent->left = NULL;
            }
            if(last->parent->right == last)
            {
                last->parent->right = NULL;
            }
            heapify(h->root, h);
        }
        free(last);
	h->size--;
	return value;
}

void levelOrderh(HEAP *h, FILE* fp, BSTNODE* root){
	if (root == NULL) return;
		//struct Queue *q = newQUEUEnh(h->display, h->free);
		QUEUE *qu = newQUEUE(h->display,0);
		int nodesInCurrentLevel = 1;
  		int nodesInNextLevel = 0;
		enqueue(qu,root);
		while (sizeQUEUE(qu) > 0){
		BSTNODE *node = dequeue(qu);//returnToph(q);
                //Dqueueh(q);
		nodesInCurrentLevel--;
		if (node){
		h->display(node->value, fp);
		if (nodesInCurrentLevel > 0) fprintf(fp," ");

		if (node->left != NULL){
		//inqueueh(q,node->left );
		enqueue(qu,node->left);
		nodesInNextLevel += 1;
		}
		if (node->right != NULL){	
		enqueue(qu,node->right);
		nodesInNextLevel += 1;
		}
		}
		if (nodesInCurrentLevel == 0){
		fprintf(fp,"\n");
		nodesInCurrentLevel = nodesInNextLevel;
		nodesInNextLevel = 0;
		}
		}
	freeQUEUE(qu);
}

void heap_preorder (BSTNODE *rt, HEAP*h, FILE *fp) {
if (rt == NULL) return; // Empty subtree - do nothing 
  fprintf(fp," [");
  h->display(rt->value, fp); 
  heap_preorder(rt->left, h, fp);    // Process all nodes in left
  heap_preorder(rt->right, h, fp);   // Process all nodes in right
  fprintf (fp,"]");
}

void displayHEAP(HEAP *h, FILE *fp){
if (h->root == NULL) {
fprintf(fp,"[ ]");
return;
}
fprintf (fp,"[");
    h->display(h->root->value, fp);
   
    heap_preorder(h->root->left, h, fp);
    
    heap_preorder(h->root->right, h, fp);
    fprintf (fp,"]");
}

void displayHEAPdebug(HEAP *h, FILE *fp){
fprintf (fp,"heap size: %d\n",sizeHEAP(h));
fprintf (fp,"bst size: %d\n",sizeHEAP(h));
levelOrderh(h, fp, h->root);
}

void freeheap(HEAP *h, BSTNODE *node){
	if (node != NULL){
	freeheap(h,node->right);
	if (h->free != NULL){
        h->free(node->value);
	}
	freeheap(h,node->left);
	h->size--;
	free(node);
        }
}

void freeHEAP(HEAP *h){
 freeQUEUE(q);
 freeSTACK(stack);
 freeheap(h,h->root);
 free(h);
}
