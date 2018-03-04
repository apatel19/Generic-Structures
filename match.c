#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[]) {

    FILE* testFile = fopen(argv[1], "r");
    FILE* myFile = fopen(argv[2], "r");

    char currChar = fgetc(testFile);
    char myChar = fgetc(myFile);

    printf("Comparing character by character...\n");

    int flag = 0;

    while(!feof(testFile) && !feof(myFile)) {
        if(currChar != myChar) {
            flag = 1;
        }

        if(testFile != 0) {
            currChar = fgetc(testFile);
        }

        if(myFile != 0) {
            myChar = fgetc(myFile);
        }
    }

    printf("Done comparing...\n");

    if(flag == 0) {
        printf("Both the files have the same input.\n");
    }

    else {
        printf("The files have a discrepancy.\n");
    }
    
    fclose(testFile);
    fclose(myFile);

return 0;
}
