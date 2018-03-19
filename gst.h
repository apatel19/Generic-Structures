 /*** green binary search tree class ***/

    #ifndef __GST_INCLUDED__
    #define __GST_INCLUDED__

    #include <stdio.h>

    typedef struct gst GST;

    extern GST *newGST(
        void (*)(void *,FILE *),           //display
        int (*)(void *,void *),           //comparator
        void (*)(void *));                 //freeing function
    extern void insertGST(GST *,void *); //Done
    extern int findGSTcount(GST *,void *); //Done
    extern void *findGST(GST *,void *); //Done 
    extern void *deleteGST(GST *,void *); //Done
    extern int sizeGST(GST *); //Done
    extern int duplicates(GST *); // Done
    extern void statisticsGST(GST *,FILE *); //Done 
    extern void displayGST(GST *,FILE *); //Done
    extern void displayGSTdebug(GST *,FILE *); //
    extern void freeGST(GST *);

    #endif
