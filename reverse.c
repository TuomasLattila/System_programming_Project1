#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

//struct for linked list containing the rows
typedef struct rows {
    char *aRow;
    struct rows *nextRow;
}ROWS;

ROWS *readFile(ROWS *pStart, char *pFileName);
ROWS *readInput(ROWS *pStart);
ROWS *addToList(ROWS *pStart, char *pRow);
void writeInReversedOrder(ROWS *pStart, FILE *pFileName);
ROWS *freeMemory(ROWS *pStart);

int main(int argc, char *argv[]) {
    
    if (argc > 3) { //Tests if too many arguments given
      fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    };

    ROWS *pStart = NULL; //start of the linked list
    struct stat in, out;
    
    switch (argc) {
        case 1: //No input or output file given
            pStart = readInput(pStart);
            writeInReversedOrder(pStart, stdout);
            break;    
        case 2: //Input file given
            pStart = readFile(pStart, argv[1]);
            writeInReversedOrder(pStart, stdout);
            break;
        case 3: //Input and output files given
	  stat(argv[1], &in);
	  stat(argv[2], &out);
            if (in.st_ino == out.st_ino) { //test if the input and output files are the same
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            };
            pStart = readFile(pStart, argv[1]);
            FILE *file;
            if ((file = fopen(argv[2], "w")) == NULL)    {
                fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
                exit(1);
            };
            writeInReversedOrder(pStart, file); //write to the output file
            fclose(file);
            break;
        default:
            break;
    };
    pStart = freeMemory(pStart); //free the linkedlist
    return 0;
};

ROWS *readFile(ROWS *pStart, char *pFileName) {
    FILE *file;
    char *pRow = NULL;
    size_t len = 0;
    __ssize_t read;

    if ((file = fopen(pFileName, "r")) == NULL)    { //opens the input file
        fprintf(stderr, "reverse: cannot open file '%s'\n", pFileName);
        exit(1);
    };

    while((read = getline(&pRow, &len, file)) != -1) { //reads lines and appends them to the linked list
        pStart = addToList(pStart, pRow);
    };
    fclose(file);
    free(pRow); 
    return pStart;
}

ROWS *readInput(ROWS *pStart) {
    char *pRow = NULL;
    size_t len = 0;
    __ssize_t read;

    //printf("Write multiple lines (end: ctrl+D):\n\n");
    while ((read = getline(&pRow, &len, stdin)) != -1) { //read from the stdin and append row to list
        pStart = addToList(pStart, pRow);
    };
    if (feof(stdin)) { //test if writing was ended
        //printf("\nWriting ended.\n\nLines in reversed:\n\n");
    } else if (ferror(stdin)) {
        fprintf(stderr, "Reading from stdin failed");
        free(pRow);
        exit(1);
    }
    
    free(pRow);
    return pStart;
}

ROWS *addToList(ROWS *pStart, char *pRow)   {
    ROWS *pNew = NULL;

    if ((pNew = (ROWS *)malloc(sizeof(ROWS))) == NULL)  { // allocates memroy for new ROW struct
        fprintf(stderr, "malloc failed\n");
        exit(1);
    };

    if ((pNew->aRow = (char *)malloc(strlen(pRow)+1)) == NULL)  { // allocates memory for the row size and adds it to the new ROW
        fprintf(stderr, "malloc failed\n");
        exit(1);
    };

    pNew->nextRow = NULL;
    strcpy(pNew->aRow, pRow);

    if (pStart == NULL) {
        pStart = pNew;
    } else {
        pNew->nextRow = pStart; //appends the new row to the start of the list (this way the rows are automatically in reversed order)
        pStart = pNew;
    }
    return pStart;
};

//print into file and take the file as an argument. (output.txt or stdout)
void writeInReversedOrder(ROWS *pStart, FILE *pFileName) {
    ROWS *ptr = pStart;
    while (ptr != NULL) {
        fprintf(pFileName, "%s", ptr->aRow); //print the linked list
        ptr = ptr->nextRow;
    }
    return;
};

ROWS *freeMemory(ROWS *pStart)  { //frees the linked list
    ROWS *ptr = pStart;
    while (ptr != NULL) {
        pStart = ptr->nextRow;
        free(ptr->aRow);
        free(ptr);
        ptr = pStart;
    }
    return (pStart = NULL);
};