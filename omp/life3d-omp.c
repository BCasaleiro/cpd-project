#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include"list.h"
#include"tree.h"
#include"aux.h"

omp_lock_t lck_a;

int main(int argc, char *argv[]) {
    /* FILE *fp; */
    float start = omp_get_wtime();
    int th=omp_get_max_threads();
    Tree ****hash;
    char name[246];
    int i, k, n;
    float end, diff;
    Row **insert, **delete;

    /** Initialize lock a */
    omp_init_lock(&lck_a);

    if (argc != 3) {
        printf("Usage: life3d-omp <infile> <iterations>.\n");
        exit(-1);
    }

    /** Get testcase filename */
    strcpy(name, argv[1]);

    /** Get number of generations */
    k = strtol(argv[2],NULL,10);

    hash = (Tree****)malloc(sizeof(Tree****));
    //Tree ****hash1 = (Tree****)malloc(sizeof(Tree****));

    /** Open output file */
    /* fp = fopen("result.out", "w"); */

    /** Create an array of Linked-Lists
        For each availabre threads create a Linked-List */
    insert=(Row**)malloc(th*sizeof(Row*));
    for (i = 0; i < th; i++) {
        insert[i]=(Row*)malloc(sizeof(Row));
    }

    /** Create an array of Linked-Lists
        For each availabre threads create a Linked-List */
    delete=(Row**)malloc(th*sizeof(Row*));
    for (i = 0; i < th; i++) {
        delete[i]=(Row*)malloc(sizeof(Row));
    }

    /** Read File */
    n = readFile(hash, name);
    for (i = 0; i < k; i++){
        if (i==0) {
        }else{
            //hash=hash1;
            //hash1 = (Tree****)malloc(sizeof(Tree****));
        }
        //printTree(hash, n);
        /** Compute next generation */
        nextGen(hash, insert, delete, n);
        //freeTree(hash, n);
    }

    /** Print tree to output file */
    /* fprintTree(hash,n, fp); */
    /* fclose(fp); */

    /** Print tree to stdout */
    printTree(hash, n);

    /** Free Linked Lists */
    for (i = 0; i < th; i++) {
        free(insert[i]);
        free(delete[i]);
    }
    /** Free Arrays */
    free(insert);
    free(delete);
    /** Free Tree */
    freeTree(hash, n);

    /** Initialize lock a */
    omp_destroy_lock(&lck_a);

    /** Compute time spent */
    end = omp_get_wtime();
    diff = end - start;
    printf("%f\n", diff);

    return 0;
}
