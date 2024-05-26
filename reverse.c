#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rows {
    char *aRow;
    struct rows *nextRow;
}ROWS;

ROWS *readFile(ROWS *pStart, char *pFileName);
ROWS *readInput(ROWS *pStart);
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
            pStart = readInput(pStart);
            writeInReversedOrder(pStart, stdout);
            break;    
        case 2: //Input file given
            pStart = readFile(pStart, argv[1]);
            writeInReversedOrder(pStart, stdout);
            break;
        case 3: //Input and output files given
            if (strcmp(argv[1], argv[2]) == 0) {
                fprintf(stderr, "Input and output file must differ\n");
                exit(1);
            };
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
        pStart = addToList(pStart, pRow);
    };
    fclose(file);
    free(pRow);
    return pStart;
}
// this is where I left last time
ROWS *readInput(ROWS *pStart) {
    char *pRow = NULL;
    size_t len = 0;
    __ssize_t read;

    printf("Write multiple lines (end: ctrl+D):\n\n");
    while ((read = getline(&pRow, &len, stdin)) != -1) {
        pStart = addToList(pStart, pRow);
    };
    if (feof(stdin)) {
        printf("\nWriting ended.\n\nLines in reversed:\n\n");
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

    if ((pNew = (ROWS *)malloc(sizeof(ROWS))) == NULL)  {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    };

    if ((pNew->aRow = (char *)malloc(strlen(pRow)+1)) == NULL)  {
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