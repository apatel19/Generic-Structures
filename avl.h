/*** AVL binary search tree class ***/

    #ifndef __AVL_INCLUDED__
    #define __AVL_INCLUDED__

    #include <stdio.h>

    typedef struct avl AVL;

    extern AVL *newAVL(
        void (*)(void *,FILE *),           //display
        int (*)(void *,void *),            //comparator
        void (*)(void *));                 //freeing function
    extern void insertAVL(AVL *,void *); // Done
    extern int findAVLcount(AVL *,void *); //Done
    extern void *findAVL(AVL *,void *); //Done
    extern void *deleteAVL(AVL *,void *);
    extern int sizeAVL(AVL *); //Done
    extern int duplicatesAVL(AVL *); //Done
    extern void statisticsAVL(AVL *,FILE *); //Done
    extern void displayAVL(AVL *,FILE *); //Done
    extern void displayAVLdebug(AVL *,FILE *); //Done
    extern void freeAVL(AVL *); 

    #endif
