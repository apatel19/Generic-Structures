#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include <stdbool.h>
#include <assert.h>
#include "queue.h"


static bool isLeaf(BSTNODE *x);
static bool isRoot(BSTNODE *x, BSTNODE *root);
static void genericSwap(BSTNODE *a, BSTNODE *b);
static BSTNODE *successor(BSTNODE *n);
static BSTNODE *predecessor(BSTNODE *n);
static int minDepth(BSTNODE *root);
static int maxDepth(BSTNODE *root);
static int min(int a, int b);

struct bstnode {
    void *value;
    BSTNODE *left, *right, *parent;
};

struct bst {
    BSTNODE *root;
    int size;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*swap)(BSTNODE *, BSTNODE *);
    void (*free) (void *v);
};

static void genericSwap(BSTNODE *a, BSTNODE *b) {
    void *vtemp = a->value;
    a->value = b->value;
    b->value = vtemp;
}

BST *newBST(
        void (*d)(void *v,FILE *fp),           //display
        int (*c)(void *v,void *f),            //comparator
        void (*s)(BSTNODE *v,BSTNODE *f),     //swapper
        void (*f)(void *v)){
    BST *newTree = (BST *)malloc(sizeof(BST));
    assert(newTree != 0);
    newTree->root = 0;
    newTree->size = 0;
    newTree->display = d;
    newTree->compare = c;
    newTree->free = f;
    if (s != NULL) {
        newTree->swap = s;
    }
    else {
        newTree->swap = genericSwap;
    }
    return newTree;
}

void *getBSTNODEvalue(BSTNODE *n){
	return n->value;	
}

void setBSTNODEvalue(BSTNODE *n, void *value) {
    n->value = value;
}

BSTNODE *getBSTNODEleft(BSTNODE *n) {
    return n->left;
}

void setBSTNODEleft(BSTNODE *n, BSTNODE *replacement) {
    n->left = replacement;
}

BSTNODE *getBSTNODEright(BSTNODE *n) {
    return n->right;
}

void setBSTNODEright(BSTNODE *n, BSTNODE *replacement) {
    n->right = replacement;
}

BSTNODE *getBSTNODEparent(BSTNODE *n) {
    return n->parent;
}

void setBSTNODEparent(BSTNODE *n, BSTNODE *replacement) {
    n->parent = replacement;
}


BSTNODE *getBSTroot(BST *t) {
    return t->root;
}

void setBSTroot(BST *t, BSTNODE *replacement) {
    t->root = replacement;
}

void setBSTsize(BST *t,int s) {
    t->size = s;
}

int sizeBST(BST *t) {
   if (t->root != NULL)
    return t->size;
   else return 0;
}

static bool isLeaf(BSTNODE *candidate) {
    return candidate->left == NULL && candidate->right == NULL;
}

BSTNODE *newBSTNODE(void *value) {
    BSTNODE *newNode = (BSTNODE *)malloc(sizeof(BSTNODE));
    newNode->value = value;
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}


BSTNODE *insertBST(BST *t, void *value) {
    assert(t != 0);
    BSTNODE *x = t->root;
    BSTNODE *y = NULL;
    BSTNODE *z = newBSTNODE(value);
    while (x != NULL) {
        y = x;
        if (t->compare(z->value, x->value) < 0) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == NULL) {
        t->root = z;
    }
    else if (t->compare(z->value, y->value) < 0) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    t->size = t->size + 1;
    return z;
}

static BSTNODE *treeMin(BSTNODE *a);
static BSTNODE *treeMax(BSTNODE *a);


static BSTNODE *treeMin(BSTNODE *x) {
    BSTNODE *n = x;
    while (n->left != NULL) {
        n = n->left;
    }
    return n;
}

static BSTNODE *treeMax(BSTNODE *x) {
    BSTNODE *n = x;
    while (n->right != NULL) {
        n = n->right;
    }
    return n;
}

static BSTNODE *successor(BSTNODE *n) {
    BSTNODE *x = n;
    if (x->right != NULL) {
	return treeMin(x->right);
    }
    return NULL; 
}

static BSTNODE *predecessor(BSTNODE *n) {
    BSTNODE *x = n;
    if (x->left != NULL) {
        return treeMax(x->left);
    }
    return NULL;
}

static bool isRoot(BSTNODE *x, BSTNODE *root) {
    return x == root;
}

BSTNODE *findBST(BST *t, void *value) {
    BSTNODE *x = t->root;
    while (x != NULL && t->compare(value, x->value) != 0) {
	if (t->compare(value, x->value) < 0) {
            x = x->left;
        }
        else x = x->right;
    } 
    return x;
}

BSTNODE *deleteBST(BST *t, void *value) {
    BSTNODE *candidate = findBST(t, value);
    if (candidate == NULL) {
        printf("Value ");
        t->display(stdout, value);
        printf(" not found.\n");
        return NULL;
    }
    BSTNODE *leaf = swapToLeafBST(t, candidate);
    pruneLeafBST(t, leaf);
    t->size -= 1;
    return leaf;
}

BSTNODE *swapToLeafBST(BST *t, BSTNODE *node) {
BSTNODE *x = node;
while (!isLeaf(x)) {
        BSTNODE *suc = successor(x);
        if (suc != NULL) {
            t->swap(suc, x);
            x = suc;
        }
        else {
            BSTNODE *pre = predecessor(x);
            t->swap(pre, x);
            x = pre;
        }
    }
    return x;
}

void pruneLeafBST(BST *t, BSTNODE *leaf) {
    if (isRoot(leaf, t->root)) {
	t->root = 0;
    }
    else if (leaf == leaf->parent->left) {
        leaf->parent->left = 0;
    }
    else leaf->parent->right = 0;
    	
}


void preorder (BSTNODE *rt, BST*tree, FILE *fp) {
if (rt == NULL) return; // Empty subtree - do nothing 
  fprintf(fp," [");
  tree->display(rt->value, fp);
  preorder(rt->left, tree, fp);    // Process all nodes in left
  preorder(rt->right, tree, fp);   // Process all nodes in right
  fprintf (fp,"]");
}

void displayBST(BST *t, FILE *fp) {
   if (t->root == NULL){
        fprintf (fp,"[]"); 
	return;
       }
    fprintf (fp,"[");
    t->display(t->root->value, fp);
    preorder(t->root->left, t, fp);
    preorder(t->root->right, t, fp);
    fprintf (fp,"]");
}	
   
static int min(int a, int b) {
    return a < b ? a : b;
}
static int minDepth(BSTNODE *root) {
    if (root == NULL) return -1;
    if (root->left == NULL || root->right == NULL) return 0;
    if (!root->left) return minDepth(root->right) + 1;
    if (!root->right) return minDepth(root->left) + 1;
    
    return min(minDepth(root->left), minDepth(root->right)) + 1;
}

/*static int minDepth(BSTNODE *root) {
	if (root == NULL) {
	printf("Root NULL\n");
	return -1;}
	else {
	int lDepth = minDepth(root->left);
	int rDepth = maxDepth(root->right);
	if (lDepth < rDepth) return (lDepth + 1);
	else return (rDepth + 1);
	}
}*/

static int maxDepth(BSTNODE *root) {
    if (root == NULL) return -1;
    else {
        int lDepth = maxDepth(root->left);
        int rDepth = maxDepth(root->right);
        if (lDepth > rDepth) return (lDepth + 1);
        else return (rDepth + 1);
    }
}

void statisticsBST(BST *t,FILE *fp){
    fprintf (fp,"Nodes: %d\n", sizeBST(t));
    fprintf (fp,"Minimum depth: %d\n", minDepth(t->root));
    fprintf (fp,"Maximum depth: %d\n", maxDepth(t->root));
}

void levelOrder(BST *h, FILE* fp, BSTNODE* root){
        if (root == NULL) return;
                //struct Queue *q = newQUEUEn(h->display, h->free);
		 QUEUE *q = newQUEUE(h->display,h->free);
		int nodesInCurrentLevel = 1;
                int nodesInNextLevel = 0;
                enqueue(q,root); 
		while (sizeQUEUE(q) > 0){
                BSTNODE *node = dequeue(q);
                //Dqueue(q);
                nodesInCurrentLevel--;
		if (node){
                h->display(node->value, fp);
                if (nodesInCurrentLevel > 0) fprintf(fp," ");
	
                if (node->left != NULL){
                enqueue(q,node->left );
                nodesInNextLevel += 1;
                }
                if (node->right != NULL){
                enqueue(q,node->right);
                nodesInNextLevel += 1;
                }
                }
                if (nodesInCurrentLevel == 0){
                fprintf(fp,"\n");
                nodesInCurrentLevel = nodesInNextLevel;
                nodesInNextLevel = 0;
                }
                }
        freeQUEUE(q);
}

void levelOrderDecorate(BST *h, FILE *fp, BSTNODE *root){
	if (root == NULL) return;
		QUEUE *q = newQUEUE(h->display, h->free);
		int nodesInCurrentLevel = 1;
		int nodesInNextLevel = 0;
		bool printLevel = true;
		int levelNumbers = 0;
		enqueue(q,root);
		while (sizeQUEUE(q) > 0){
		BSTNODE *node = dequeue(q);	
		nodesInCurrentLevel--;
		if(node){
		if (printLevel == true) fprintf (fp,"%d: ", levelNumbers);
		printLevel = false;
		if (node->left == NULL && node->right == NULL) fprintf (fp, "=");
		h->display(node->value, fp);
		//if (nodesInCurrentLevel > 0) fprintf(fp, " ");
	
		if (node->parent != NULL) {
		fprintf (fp, "(");
		h->display(node->parent->value, fp);
		fprintf (fp, ")");
		}
		else {
		fprintf (fp, "(");
		h->display (node->value, fp);
		fprintf (fp, ")");
		}
		if (node->parent == NULL) fprintf (fp,"X"); 
		else if ((node->parent->right != NULL) && (h->compare(node->parent->right->value, node->value)==0)) fprintf (fp,"R");
		else if ((node->parent->left != NULL) && (h->compare(node->parent->left->value, node->value) ==0)) fprintf (fp,"L");
		if (nodesInCurrentLevel > 0) fprintf(fp, " ");
		if (node->left != NULL){
		enqueue(q, node->left);
		nodesInNextLevel += 1;
		}
		if (node->right != NULL){
		enqueue(q, node->right);
		nodesInNextLevel += 1;
		}
		}
		if (nodesInCurrentLevel == 0){
		fprintf(fp, "\n");
		printLevel = true;
		levelNumbers++;
		nodesInCurrentLevel = nodesInNextLevel;
		nodesInNextLevel = 0;
		}
		}
	freeQUEUE(q);
}

void displayBSTdecorated(BST*t, FILE *fp){
	if (t->root == NULL) return;
	levelOrderDecorate(t, fp, t->root);
}

void displayBSTdebug(BST *t,FILE *fp){
if (t->root == NULL) {
return;
}
levelOrder(t,fp,t->root);
}

void freeBSTNODE(BSTNODE *n,void (*f)(void *v)){
    if (f != NULL) {
        f(n->value);
        free(n);
        }
    else {
        free(n);
        } 
}


void freebst (BST *t, BSTNODE* node){
	if (node != NULL){
	 freebst(t,node->right);
	 if (t->free != NULL){
          t->free(node->value);
	 }
	 freebst(t,node->left);
	 t->size--;
         free(node);
	}
}

void freeBST(BST *t){
        freebst(t, t->root);
        free(t);
}
