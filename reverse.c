#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rows {
    char *aRow;
    struct rows *nextRow;
}ROWS;

ROWS *readFile(ROWS *pStart, char *pFileName);
ROWS *addToList(ROWS *pStart, char *pRow);
void writeInReversedOrder(ROWS *pStart, FILE *pFileName);
ROWS *freeMemory(ROWS *pStart);

/*At the moment the code expects one or two commandline argument which
are the names of the input file and the output file.*/

int main(int argc, char *argv[]) {
    
    if (argc > 3) { //Tests if too many arguments given
        printf("usage: reverse <input> <output>\n");
        exit(1);
    };

    ROWS *pStart = NULL;

    switch (argc) {
        case 1: //No input or output file given
            
            break;    
        case 2: //Input file given
            pStart = readFile(pStart, argv[1]);
            writeInReversedOrder(pStart, stdout);
            break;
        case 3: //Input and output files given
            pStart = readFile(pStart, argv[1]);
            FILE *file;
            if ((file = fopen(argv[2], "w")) == NULL)    {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
            };
            writeInReversedOrder(pStart, file);
            fclose(file);
            break;
        
        default:
            break;
    };
    pStart = freeMemory(pStart);
    return 0;
};

ROWS *readFile(ROWS *pStart, char *pFileName) {
    FILE *file;
    char *pRow = NULL;
    size_t len = 0;
    __ssize_t read;

    if ((file = fopen(pFileName, "r")) == NULL)    {
        fprintf(stderr, "error: cannot open file '%s'\n", pFileName);
        exit(1);
    };

    while((read = getline(&pRow, &len, file)) != -1) {
        //printf("%s", pRow);
        pStart = addToList(pStart, pRow);
    };
    fclose(file);
    free(pRow);
    return pStart;
}

ROWS *addToList(ROWS *pStart, char *pRow)   {
    ROWS *pNew = NULL;

    if ((pNew = (ROWS *)malloc(sizeof(ROWS))) == NULL)  {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    };

    if ((pNew->aRow = (char *)malloc(sizeof(pRow)+1)) == NULL)  {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    };

    pNew->nextRow = NULL;
    strcpy(pNew->aRow, pRow);

    if (pStart == NULL) {
        pStart = pNew;
    } else {
        pNew->nextRow = pStart;
        pStart = pNew;
    }
    return pStart;
};

//print into file and take the file as an argument. (output.txt or stdout)
void writeInReversedOrder(ROWS *pStart, FILE *pFileName) {
    ROWS *ptr = pStart;
    while (ptr != NULL) {
    fprintf(pFileName, "%s", ptr->aRow);
    ptr = ptr->nextRow;
    }
    return;
};

ROWS *freeMemory(ROWS *pStart)  {
    ROWS *ptr = pStart;
    while (ptr != NULL) {
        pStart = ptr->nextRow;
        free(ptr->aRow);
        free(ptr);
        ptr = pStart;
    }
    return (pStart = NULL);
};