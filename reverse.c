#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rows {
    char *aRow;
    struct rows *nextRow;
}ROWS;

ROWS *readFile(ROWS *pStart, char *pFileName);
ROWS *addToList(ROWS *pStart, char *pRow);
void printInReversedOrder(ROWS *pStart);
ROWS *freeMemory(ROWS *pStart);

/*At the moment the code expects one commandline argument which
is the name of the input file.*/

int main(int argc, char *argv[]) {
    ROWS *pStart = NULL;

    pStart = readFile(pStart, argv[1]);
    printInReversedOrder(pStart);

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

void printInReversedOrder(ROWS *pStart) {
    ROWS *ptr = pStart;
    while (ptr != NULL) {
        printf("%s", ptr->aRow);
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