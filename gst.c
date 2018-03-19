
#include <stdlib.h>
#include "bst.h"
#include "gst.h"

struct gst {
	BST *bst;
	int freqTotal;
	void (*display)(void *, FILE *);
        int (*compare)(void *, void *);
        void (*swap)(BSTNODE *, BSTNODE *);
        void (*free) (void *v);
};

typedef struct gstv{
	void *value;
	int freq;
	void (*display) (void *, FILE *);
	int (*compare) (void *, void *);
	void (*free) (void *v);
} GSTV;

static GSTV *newGSTV(GST *gst, void *value);
static void displayGSTV(void *v, FILE *fp);
static int compareGSTV(void *a, void *b);
static void swapGSTV(BSTNODE *a, BSTNODE *b);
static void freeGSTV(void *f);

GST *newGST( void (*display)(void *,FILE *), int (*compare)(void *,void *), void (*free)(void *)){
        GST *gst = (GST *) malloc(sizeof(GST));
        gst->display = display;
        gst->compare = compare;
        gst->free = free;
        gst->bst = newBST(displayGSTV, compareGSTV, swapGSTV, freeGSTV);
        gst->freqTotal = 0;
        return gst;
}

void insertGST(GST *gt, void* value){
        GSTV *gtvalue = newGSTV(gt, value);
        BSTNODE *searchNode = findBST(gt->bst, gtvalue);
        if (searchNode != NULL){
	        GSTV *searchVal = getBSTNODEvalue(searchNode);
        	searchVal->freq += 1;
		gt->free(gtvalue->value);
		free(gtvalue);
	}
        else {
        	insertBST(gt->bst, gtvalue);
	}
        gt->freqTotal += 1;
}

static GSTV *newGSTV(GST *gt, void *value){
        GSTV *gstv = (GSTV *) malloc (sizeof(GSTV));
        gstv->value = value;
        gstv->freq = 1;
        gstv->display = gt->display;
        gstv->compare = gt->compare;
        gstv->free = gt->free;
        return gstv;
}

int findGSTcount(GST *gt ,void *v){
        GSTV *gtvalue = newGSTV(gt, v);
        BSTNODE *searchNode = findBST(gt->bst, gtvalue);
        if (searchNode == 0) {
		gt->free(gtvalue->value);
		free(gtvalue);
		return 0;
        }
	else {
		GSTV *searchVal = getBSTNODEvalue(searchNode);
         	gt->free(gtvalue->value);
	 	free(gtvalue);
		return searchVal->freq;
	}
}

void *findGST(GST *gt,void *v){
        GSTV *gtvalue = newGSTV(gt, v);
        BSTNODE *searchNode = findBST(gt->bst, gtvalue);
        if (searchNode == 0){
	//gt->free(gtvalue->value);
		free(gtvalue);
		return NULL;
	}
        else {
	//gt->free(gtvalue->value);
		free(gtvalue);
		return v;
	}
}

int sizeGST(GST *gt){
        return sizeBST(gt->bst);
}

int duplicates(GST *gt){	
	return gt->freqTotal - sizeBST(gt->bst);
}

void *deleteGST(GST *gt,void *value){
	GSTV *gtvalue = newGSTV(gt, value);
        BSTNODE *searchNode = findBST(gt->bst, gtvalue);
	void *returnThis = NULL;
	if (searchNode == 0){
        	printf("Value ");
        	gtvalue->display(value,stdout);
        	printf(" not found.\n");
		free(gtvalue);
        	return NULL;
        }
        GSTV *searchVal = getBSTNODEvalue(searchNode);
        if (searchVal -> freq > 1){
        	searchVal->freq -= 1;
        }
        else {
    		searchNode = deleteBST(gt->bst, gtvalue);
		GSTV* v = getBSTNODEvalue(searchNode);
		returnThis = v->value;
		free(searchNode);
		free(v);
	}
        gt->freqTotal -= 1;
	free(gtvalue);
	return returnThis;
}


void displayGST(GST *gt,FILE *fp){
	if (sizeBST(gt->bst) != 0)
	displayBSTdecorated(gt->bst, fp);
	else fprintf(fp,"EMPTY\n");	
}

void statisticsGST(GST *gt,FILE *fp){
	printf ("Duplicates: %d\n", duplicates(gt));
        statisticsBST(gt->bst, fp);
}

static void displayGSTV(void *value, FILE *fp){
        GSTV *gtval = (GSTV *)value;
        gtval->display (gtval->value, fp);
        if (gtval->freq > 1) fprintf (fp, "[%d]", gtval->freq);
	
}

void displayGSTdebug(GST *gt,FILE *fp){
	displayBST(gt->bst, fp);
}

static int compareGSTV(void *a, void *b) {
    GSTV *gtva = (GSTV *)a;
    GSTV *gtvb = (GSTV *)b;
    return gtva->compare(gtva->value, gtvb->value);
}

static void swapGSTV(BSTNODE *a, BSTNODE *b) {
    GSTV *gtva = getBSTNODEvalue(a);
    GSTV *gtvb = getBSTNODEvalue(b);
    void *vtemp = gtva->value;
    gtva->value = gtvb->value;
    gtvb->value = vtemp;
    int ftemp = gtva->freq;
    gtva->freq = gtvb->freq;
    gtvb->freq = ftemp;
}

static void freeGSTV(void *f){
	GSTV *gtva = getBSTNODEvalue(f);
	free(gtva);
	free(f);
}

void freeGST(GST *gt){
	freeBST(gt->bst);
	free(gt);
}
