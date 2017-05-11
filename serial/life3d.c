#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"
#include"tree.h"
#include"aux.h"

int main(int argc, char *argv[]) {
    /* FILE *fp; */
    char name[246];
    int i, k, n;
    Row *insert;
    Tree ****hash;
    Tree ****hash1;


    if (argc != 3) {
        printf("Usage: life3d <infile> <iterations>.\n");
        exit(-1);
    }

    /** Get testcase filename */
    strcpy(name, argv[1]);

    /** Get number of generations */
    k = strtol(argv[2],NULL,10);

    hash = (Tree****)malloc(sizeof(Tree****));
    hash1 = (Tree****)malloc(sizeof(Tree****));

    /** Open output file */
    /* fp = fopen("result.out", "w"); */

    /** Allocate and initialize Linked List */
    insert=(Row*)malloc(sizeof(Row));
    init(insert);

    /** Read File */
    n = readFile(hash, name);
    for (i = 0; i < k; i++){
        /** In the first generation ignore the past one
            after that update the hash to be hash1 */
        if (i != 0) {
            hash = hash1;
            hash1 = (Tree****)malloc(sizeof(Tree****));
        }

        /** Re-Initialize Linked List */
        init(insert);
        /** Compute next generation */
        nextGen(hash, insert, n, hash1);
        /** Free Linked List */
        freeList(insert);
        /** Free past generation */
        freeTree(hash, n);
    }

    /** Print tree to output file */
    /* fprintTree(hash1,n, fp); */
    /* fclose(fp); */
    /* printf("live cells at result.out\n"); */

    /** Print tree to stdout */
    printTree(hash1,n);

    /** Free Linked List and Tree */
    free(insert);
    freeTree(hash1, n);

    return 0;
}
