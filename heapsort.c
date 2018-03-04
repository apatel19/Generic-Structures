#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "scanner.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include <stdarg.h>
#include <stdbool.h>
//#include "bst.h"
#include "heap.h"
void heapifyy(void *node, int typeDataFile);
int comparision(void* , void*, int);
int ProcessOptions(int, char **);
void Fatal(char *, ...);
char *fileName = 0;
int isD = 0;
int typeDataFile = 0;
//0 = Int
//1 = real
//2 = string

int main (int argc, char *argv[]){
int argumentIndex = 0;
    if(argc == 1) Fatal("%d arguments! \n", argc-1);

argumentIndex = ProcessOptions(argc, argv);
if (argumentIndex == argc){}

FILE *fp = fopen(fileName,"r");
if (fp == 0){
fprintf(stderr, "File is not opening\n");
exit(0);
}

HEAP *h = NULL;
	switch (typeDataFile){
	case 0: 
		if (isD == 0)
			h = newHEAP (displayINTEGER, compareINTEGER, freeINTEGER);
		else 
			h = newHEAP (displayINTEGER, rcompareINTEGER, freeINTEGER);
	
		int x = readInt(fp);
		insertHEAP(h, newINTEGER(x));
        	x = readInt(fp);
        	while (!feof(fp)){
        	insertHEAP(h, newINTEGER(x));
        	x = readInt(fp);
		}
		buildHEAP(h);
        	while(sizeHEAP(h) != 0){ 
                INTEGER *value = extractHEAP(h);
                displayINTEGER(value, stdout);
		if (sizeHEAP(h) != 0)
                	printf (" ");
		}
		
		break;
	case 1:
		if(isD == 1)
			h = newHEAP (displayREAL, rcompareREAL, freeREAL);
		else 
			h = newHEAP (displayREAL, compareREAL, freeREAL);
		
		double y = readReal(fp);
		insertHEAP(h, newREAL(y));
		y = readReal(fp);
		while (!feof(fp)){
		insertHEAP(h, newREAL(y));
		y = readReal(fp);
		}
	
		buildHEAP(h);
        	while(sizeHEAP(h) != 0){ 
                REAL *value = extractHEAP(h);
                displayREAL(value, stdout);
		if (sizeHEAP(h) != 0)
                	printf (" ");
		}
		break;
	case 2: 
		if (isD == 1)
			h = newHEAP (displaySTRING, rcompareSTRING, freeSTRING);
		else 
			h = newHEAP (displaySTRING, compareSTRING, freeSTRING);
		
		bool check = true;      
        	char *z;        
        	if (check == stringPending(fp)) 
                	 z = readString(fp);
        	else 
                	 z = readToken(fp);
		
		insertHEAP(h, newSTRING(z));
		if (check == stringPending(fp)) 
                	z = readString(fp);
        	else 
                	z = readToken(fp);

		while (!feof(fp)){
		insertHEAP(h, newSTRING(z));
		if (check == stringPending(fp)) 
        	        z = readString(fp);
	        else 
                	z = readToken(fp);
		}
		
		buildHEAP(h);
        	while(sizeHEAP(h) != 0){ 
                STRING *value = extractHEAP(h);
                displaySTRING(value, stdout);
		if (sizeHEAP(h) != 0)
                	printf (" ");
		}	
		break;
	default:
		if (isD == 0)
                        h = newHEAP (displayINTEGER, compareINTEGER, freeINTEGER);
                else
                        h = newHEAP (displayINTEGER, rcompareINTEGER, freeINTEGER);

                int a = readInt(fp);
                insertHEAP(h, newINTEGER(a));
                a = readInt(fp);
                while (!feof(fp)){
                insertHEAP(h, newINTEGER(a));
                a = readInt(fp);
                }
		buildHEAP(h);
        	while(sizeHEAP(h) != 0){ 
                INTEGER *value = extractHEAP(h);
                displayINTEGER(value, stdout);
		if (sizeHEAP(h) != 0)
                	printf (" ");
		}
		break;
	}
    printf("\n");
return 0;
}

void Fatal(char *fmt, ...)
{   
    va_list ap;
    
    fprintf(stderr, "An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    
    exit(-1);
}


int ProcessOptions(int argc, char **argv)
{
    int argumentIndex;
    int argumentUsed;
    int separateArgument;
    char *argument;

    argumentIndex = 1;

    while(argumentIndex < argc && *argv[argumentIndex] == '-')
    {
        if(argv[argumentIndex][1] == '\0') return argumentIndex;

        separateArgument = 0;
        argumentUsed = 0;
        argument = 0;
        argument++;

        if(argv[argumentIndex][2] == '\0')
        {
            argument = argv[argumentIndex+1];
            separateArgument = 1;
        }
        else
        {
            argument = argv[argumentIndex]+2;
        }
        switch(argv[argumentIndex][1])
        {
        case 'v':
            	printf ("Apurva Patel\nCS 201 Assignment 1\n\nMy implementation of a binary tree as a heap uses queue(two way pointer). Using heapify takes at \nmost log n time because it has to iterate left and right for \neach node, then it has to do this per node, so making\nthe total time complexity n log n.\n");
		exit(0);
        case 'D': 
            isD = 1;
            break;
        
	case 'I':
	    isD = 0;
	    break;

	case 'r':
	    typeDataFile = 1;
	    break;	
	
	case 'i':
	    typeDataFile = 0;
	    break;
	
	case 's':
            typeDataFile = 2;
            break;
	default:
            Fatal("option %s not understood\n", argv[argumentIndex]);
        }

        if(separateArgument && argumentUsed)
        {
            ++argumentIndex;
        }
        ++argumentIndex;
    }
   
    fileName = argv[argc-1];

    return argumentIndex;
}


