 #ifndef __QUEUE_INCLUDED__
 #define __QUEUE_INCLUDED__

    #include <stdio.h>

    typedef struct queue QUEUE;

    extern QUEUE *newQUEUE(void (*d)(void *,FILE *),void (*f)(void *)); //Done 
    extern void enqueue(QUEUE *items,void *value); //Done
    extern void *dequeue(QUEUE *items); //Done
    extern void *peekQUEUE(QUEUE *items); 
    extern int sizeQUEUE(QUEUE *items); //Done
    extern void displayQUEUE(QUEUE *items,FILE *); //Done
    extern void displayQUEUEdebug(QUEUE *items,FILE *); //Done
    extern void freeQUEUE(QUEUE *items); 

    #endif
