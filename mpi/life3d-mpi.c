#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>
#include"list.h"
#include"tree.h"
#include"aux.h"

int main(int argc, char *argv[]) {
    Tree ****hash;
    char name[246];
    int i, k, n;
    int id, nprocs;
    Row *insert, *delete;

    if (argc != 3) {
        printf("Usage: life3d-omp <infile> <iterations>.\n");
        exit(-1);
    }

    /** Get testcase filename */
    strcpy(name, argv[1]);

    /** Get number of generations */
    k = strtol(argv[2],NULL,10);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    hash = (Tree****)malloc(sizeof(Tree****));

    /** Read File */
    n = readFile(hash, name, id, nprocs);
    printTree(hash, n, id);

    //insert=(Row*)malloc(sizeof(Row));
    //delete=(Row*)malloc(sizeof(Row));

    //for (i = 0; i < k; i++){
    //    /** Compute next generation */
    //    nextGen(hash, insert, delete, n);
    //}

    /** Free Linked Lists */
    //free(insert[i]);
    //free(delete[i]);

    /** Free Tree */
    freeTree(hash, n);

    MPI_Finalize();

    /** Print tree to stdout */
    //printTree(hash, n);



    return 0;
}
