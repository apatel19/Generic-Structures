#include <stdio.h>
#include "bst.h"
#include <stdlib.h>
#include "avl.h"
#include <stdbool.h>
struct avl{
        BST *bst;
        int freqTotal;
        void (*display)(void *, FILE *);
        int (*compare)(void *, void *);
        void (*swap)(BSTNODE *, BSTNODE *);
        void (*free) (void *v);
};

typedef struct avlv{
        void *value;
        int freq;
        int balance;
        int leftHeight;
        int rightHeight;
        int height;
        void (*display) (void *, FILE *);
        int (*compare) (void *, void *);
        void (*free) (void *v);
} AVLV;

//Essential Functions
static AVLV *newAVLV(AVL *avl, void* value);
static void displayAVLV(void *v, FILE *fp);
static int compareAVLV(void *a, void *b);
static void swapAVLV(BSTNODE *a, BSTNODE *b);
static void freeAVLV(void *f);

//Other Static Functions
static void setBalance (BSTNODE *x);
static int maxDepth(BSTNODE *root);

//Fixup Functions
static void insertionFixup(AVL* avl, BST *t, BSTNODE* n);
static bool checkBalance(BSTNODE *node);
static void deleteFixup(AVL *avl, BST* t, BSTNODE* n);
//Rotation functions
static BSTNODE* rotateRight(BSTNODE* a);
static BSTNODE* rotateLeft(BSTNODE* a);
static BSTNODE* rotateRightThenLeft(BSTNODE *n);
static BSTNODE* rotateLeftThenRight(BSTNODE *n);

//From avl.h file

AVL *newAVL( void (*display)(void *,FILE *), int (*compare)(void *,void *), void (*free)(void *)){
        AVL *avl = (AVL *) malloc(sizeof(AVL));
        avl->display = display;
        avl->compare = compare;
        avl->free = free;
        avl->bst = newBST(displayAVLV, compareAVLV, swapAVLV, freeAVLV);
        avl->freqTotal = 0;
        return avl;
}

void
insertAVL(AVL *avl, void *value){
        AVLV *avlvalue = newAVLV(avl, value);
        BSTNODE *searchNode = findBST(avl->bst, avlvalue);
        if (searchNode != NULL){
	        AVLV *searchVal = getBSTNODEvalue(searchNode);
        	searchVal->freq += 1;
        	avl->free(avlvalue->value);
		free(avlvalue);
	}
        else {
        	BSTNODE *x = insertBST(avl->bst,avlvalue);
		BSTNODE *parent = NULL;
		parent = getBSTNODEparent(x);
	  if (x != getBSTroot(avl->bst) && parent != NULL){      	
			insertionFixup(avl,avl->bst, parent);
		}
		else {
			setBalance(x);
		}
	}
        	avl->freqTotal += 1;
}

void *
deleteAVL(AVL *avl,void *value){
        AVLV* avlvalue = newAVLV(avl, value);
        BSTNODE* searchNode = findBST(avl->bst, avlvalue);
        void *returnThis = NULL;
        if (searchNode == NULL){
        	printf("Value ");
        	avlvalue->display(avlvalue->value, stdout);
        	printf(" not found.\n");
        	free(avlvalue->value);
		free(avlvalue);
        	return NULL;
        }
        AVLV* searchVal = getBSTNODEvalue(searchNode);
        AVLV* leafValue = NULL;
        	if (searchVal->freq > 1){
        		searchVal->freq -= 1;
        	}
        	else {
        		BSTNODE* leaf = swapToLeafBST(avl->bst, searchNode);
        		BSTNODE* leafParent = NULL;
        		leafParent = getBSTNODEparent(leaf);
        		pruneLeafBST(avl->bst,leaf);
	
		if (leafParent!=NULL && sizeBST(avl->bst)>1){
				deleteFixup(avl, avl->bst, leafParent);
        	}
		setBSTsize(avl->bst, (sizeBST(avl->bst)) - 1);
        	
			if (leaf != NULL){
                		leafValue = getBSTNODEvalue(leaf);
                		returnThis = leafValue->value;
        		}	
        		if (leaf != NULL)
                		free(leaf);
        		if (leafValue != NULL)
                		free(leafValue);
        	}
        avl->freqTotal -= 1;
	free(avlvalue);
        return returnThis;
}


void *findAVL(AVL *avl,void *v){
        AVLV *avlvalue = newAVLV(avl, v);
        BSTNODE *searchNode = findBST(avl->bst, avlvalue);
        if (searchNode == 0){
        	free(avlvalue);
        	return NULL;
        }
        else {
        	free(avlvalue);
        	return v;
        }
}

int findAVLcount(AVL *avl ,void *v){
        AVLV *avlvalue = newAVLV(avl, v);
        BSTNODE *searchNode = findBST(avl->bst, avlvalue);
        if (searchNode == 0) {
        	avl->free(avlvalue->value);
		free(avlvalue);
        	return 0;
        }
        else {
        	AVLV *searchVal = getBSTNODEvalue(searchNode);
        	avl->free(avlvalue->value);
		free(avlvalue);
        	return searchVal->freq;
        }
}

void displayAVL(AVL *avl,FILE *fp){
	if (getBSTroot(avl->bst) != NULL)
		displayBSTdecorated(avl->bst, fp);
	else fprintf(fp,"EMPTY\n");
}

void displayAVLdebug(AVL *avl,FILE *fp){
        	displayBST(avl->bst, fp);
}

int sizeAVL(AVL *avl){
        	return sizeBST(avl->bst);
}


int duplicatesAVL(AVL *gt){
        return gt->freqTotal - sizeBST(gt->bst);
}

void statisticsAVL(AVL *avl,FILE *fp){
        printf ("Duplicates: %d\n", duplicatesAVL(avl));
        statisticsBST(avl->bst, fp);
}

void freeAVL(AVL *at){
        freeBST(at->bst);
        free(at);
}

//Rotations
//Fixup Functions

static BSTNODE* getSibling(BSTNODE *currentNode){
        if (currentNode == NULL) return NULL;
        
	BSTNODE* parentOfCurrentNode = getBSTNODEparent(currentNode);
       
	if (currentNode != getBSTNODEleft(parentOfCurrentNode))
                return getBSTNODEright(parentOfCurrentNode);
        else if (currentNode != getBSTNODEright(parentOfCurrentNode))
                return getBSTNODEleft(parentOfCurrentNode);
        else return NULL;
}

static bool parentFavorsSibling (BST*t, BSTNODE *currentNode){
        BSTNODE *currNodeParent = getBSTNODEparent(currentNode);
        
	BSTNODE* currNodeSibling = NULL;
	if (currentNode != getBSTroot(t) && currentNode != NULL)
		currNodeSibling = getSibling(currentNode);
        
	AVLV *parentValue = getBSTNODEvalue(currNodeParent);

        if (currNodeSibling != NULL){
        if (parentValue->balance > 0){
                if (getBSTNODEleft(currNodeParent) != currentNode)
                        return true;
                else return false;
        }
        else if (parentValue->balance < 0){
                if (getBSTNODEright(currNodeParent) != currentNode)
                        return true;
                else return false;
        }
        else return false;
        }
        else {
        if (parentValue->balance > 0){
                if (getBSTNODEleft(currNodeParent) == NULL)
                        return true;
                else 	return false;
		}
        else if (parentValue->balance < 0){
                if (getBSTNODEright(currNodeParent) == NULL)
                        return true;
                else return false;
        }
        else return false;
        }
}

void deleteFixup(AVL* avl, BST *t, BSTNODE* n){
        //int i = 0;
        while (true){
        setBalance(n);     
        //AVLV* nvalue = NULL;
        AVLV* nvalue = getBSTNODEvalue(n);
        if (nvalue->balance == 2){
                AVLV *ll = getBSTNODEvalue(getBSTNODEleft(n));
                if (ll->leftHeight >= ll->rightHeight){
                        n = rotateRight(n);
                }
                else{
                        n = rotateLeftThenRight(n);
                }
        }
        else if (nvalue->balance == -2){
                AVLV* rr = getBSTNODEvalue(getBSTNODEright(n));
                if (rr->rightHeight >= rr->leftHeight){
                        n = rotateLeft(n);
                }
                else{
                        n = rotateRightThenLeft(n);
                }
        }
        BSTNODE* parent = getBSTNODEparent(n);
        if (n != getBSTroot(avl->bst) && parent != NULL) {
                bool pfavs = parentFavorsSibling(t,n);

                if (pfavs == true){
                        setBalance(parent);
                        break;
                }
                else {
                        n = parent;
                }
        }
        else {
                //if (sizeBST(t) != 1)
			setBSTroot(t,n);
                break;
                }
        }
}

void insertionFixup(AVL* avl, BST *t, BSTNODE* n){
	//int i = 0;
	while (true){
	setBalance(n);
        //AVLV* nvalue = NULL;
        AVLV* nvalue = getBSTNODEvalue(n);
        if (nvalue->balance == 2){
                AVLV *ll = getBSTNODEvalue(getBSTNODEleft(n));
                if (ll->leftHeight >= ll->rightHeight){
                        n = rotateRight(n);
                }
                else{
                        n = rotateLeftThenRight(n);
                }
        }
        else if (nvalue->balance == -2){
                //AVLV *rr = NULL;
                //if (getBSTNODEright(n) != NULL){
                AVLV* rr = getBSTNODEvalue(getBSTNODEright(n));
                if (rr->rightHeight >= rr->leftHeight){
                        n = rotateLeft(n);
                }
                else{
                        n = rotateRightThenLeft(n);
		}
        }	
        BSTNODE* parent = getBSTNODEparent(n);
        if (n != getBSTroot(avl->bst) && parent != NULL) {
		bool pfavs = parentFavorsSibling(t,n);
                
		if (pfavs == true){
			setBalance(parent);
			break;
        	}
		else if (checkBalance(parent) == true){
			setBalance(parent);
			n = parent;
		}
        	else {
			n = parent;
		} 
	}
	else {
		setBSTroot(t,n);
		break;
		} 
	}
}



static bool checkBalance(BSTNODE *node) {
	if (node != NULL)
	{
		AVLV* nodeValue = getBSTNODEvalue(node);
		return (nodeValue->balance == 0 || nodeValue->balance == -1 || nodeValue->balance == 1);	
	}
	return false;
}

static BSTNODE* rotateRight(BSTNODE* a){
        BSTNODE *b = NULL;
	if (getBSTNODEleft(a) != NULL)
		b = getBSTNODEleft(a);
	setBSTNODEparent(b,getBSTNODEparent(a));
        setBSTNODEleft(a,getBSTNODEright(b));

        if (getBSTNODEleft(a) != NULL){
        BSTNODE* temp = getBSTNODEleft(a);
        setBSTNODEparent(temp,a);
        }

        setBSTNODEright(b,a);
        setBSTNODEparent(a,b);

        if (getBSTNODEparent(b) != NULL){
                BSTNODE* temp = getBSTNODEparent(b);
                if (getBSTNODEright(temp) == a){
                setBSTNODEright(temp,b);
                }
                else {
                setBSTNODEleft(temp,b);
                }
        }
        setBalance(a);
        setBalance(b);
        return b;
}

static BSTNODE* rotateLeft(BSTNODE* a){
        BSTNODE* b = getBSTNODEright(a);
        setBSTNODEparent(b, getBSTNODEparent(a));
        setBSTNODEright(a, getBSTNODEleft(b));
        if (getBSTNODEright(a) != NULL){
        BSTNODE *temp = getBSTNODEright(a);
        setBSTNODEparent(temp, a);
        }
        setBSTNODEleft(b,a);
        setBSTNODEparent(a,b);
        if (getBSTNODEparent(b) != NULL){
                BSTNODE* temp = getBSTNODEparent(b);
                if (getBSTNODEright(temp) == a){
                setBSTNODEright(temp,b);
                }
                else {
                setBSTNODEleft(temp,b);
                }
        }
        setBalance(a);
        setBalance(b);
        return b;
}

static BSTNODE* rotateLeftThenRight(BSTNODE *n){
	if (getBSTNODEleft(n) != NULL)
        setBSTNODEleft(n, rotateLeft(getBSTNODEleft(n)));
        return rotateRight(n);
}

static BSTNODE* rotateRightThenLeft(BSTNODE *n){
	if (getBSTNODEright(n) != NULL)
        setBSTNODEright(n, rotateRight(getBSTNODEright(n)));
        return rotateLeft(n);
}

//Other Static Functions

static void setBalance (BSTNODE *x){
	//if (x != NULL){
        AVLV *avlv = getBSTNODEvalue(x);
	//printf ("Set Balance Value is ");
	//displayINTEGER(avlv->value,stdout);
        BSTNODE *left = NULL;
	if (getBSTNODEleft(x) != NULL)
		left = getBSTNODEleft(x);
        BSTNODE *right = NULL;
	if (getBSTNODEright(x) != NULL)
		right = getBSTNODEright(x);

	avlv->leftHeight = maxDepth(left);
	avlv->rightHeight = maxDepth(right);
        
	if (avlv->leftHeight >= avlv->rightHeight){
                avlv->height = 1+ avlv->leftHeight;
        }
        else {
                avlv->height = 1+ avlv->rightHeight;
        }
        avlv->balance = avlv->leftHeight - avlv->rightHeight;
}

static int maxDepth(BSTNODE *root) {
    if (root == NULL) return -1;
    else {
        int lDepth = maxDepth(getBSTNODEleft(root));
        int rDepth = maxDepth(getBSTNODEright(root));
        if (lDepth > rDepth){
         return (lDepth + 1);
        }
        else {
        return (rDepth + 1);
        }
 }
}

//Essential static functions
static AVLV *newAVLV(AVL *avl, void* value){
        AVLV *avlv = (AVLV *)malloc (sizeof(AVLV));
        avlv->value = value;
        avlv->freq = 1;
        avlv->balance = 0;
        avlv->height = 0;
        avlv->leftHeight = -1;
        avlv->rightHeight = -1;
        avlv->display = avl->display;
        avlv->compare = avl->compare;
        avlv->free = avl->free;
        return avlv;
}

static void displayAVLV(void *v, FILE *fp){
        if (v != NULL){
	AVLV* avalue = (AVLV *)v;
        avalue->display(avalue->value, fp);
        if (avalue->freq > 1) fprintf (fp,"[%d]", avalue->freq);
        
	if (avalue->balance < 0) fprintf(fp,"-");
        else if (avalue->balance > 0) fprintf(fp,"+");
	}
}

static void swapAVLV(BSTNODE* a, BSTNODE* b){
        AVLV *avalue = NULL;
	AVLV *bvalue = NULL;
	if (a != NULL)
		avalue = getBSTNODEvalue(a);
        if (b != NULL)
		bvalue = getBSTNODEvalue(b);
        void *temp = avalue->value;
        avalue->value = bvalue->value;
        bvalue->value = temp;
        int ftemp = avalue->freq;
        avalue->freq = bvalue->freq;
        bvalue->freq = ftemp;
}

static void freeAVLV(void *f){
	if (f != NULL){
        AVLV *avlv = getBSTNODEvalue(f);
        free(avlv);
        free(f);
	}
}

static int compareAVLV(void *a, void *b){
        AVLV *avlva = (AVLV *)a;
        AVLV *avlvb = (AVLV *)b;
        return avlva->compare(avlva->value, avlvb->value);
}
