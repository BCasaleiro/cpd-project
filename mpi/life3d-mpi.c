#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>
#include"list.h"
#include"tree.h"
#include"aux.h"

// COMS TAGS
#define UPPER_COUNT 1
#define LOWER_COUNT 2
#define UPPER_VECTOR 3
#define LOWER_VECTOR 4


int main(int argc, char *argv[]) {
    Tree ****hash;
    char name[246];
    int i, k, n;
    int id, nprocs;
    Row *insert, *delete;
	MPI_Request request_up;
	MPI_Request request_down;
	MPI_Status status_up;
	MPI_Status status_down;

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
    printTree(hash, n, id, nprocs);

    insert=(Row*)malloc(sizeof(Row));
    delete=(Row*)malloc(sizeof(Row));

    int sizef, sizei, k;
    sizef=sizei=k=0;

    int * nodesf, nodesi;

    for (i = 0; i < k; i++) {

        sizef=sizei=0;
        k=0;

        //get number of nodes
        for(int j=0; j<n; j++){
          sizei = hash[1][j]->size + sizei;
        }

        for(int j=0; j<n; j++){
          sizef = hash[BLOCK_SIZE(id, nprocs, n)+1][j]->size + sizef;
        }

        //alloc memory for sending arrays
        nodesi = (int*)malloc(sizei*sizeof(int)*2);
        nodesf = (int*)malloc(sizef*sizeof(int)*2);

        //fill the arrays with yz nodes
        for (size_t j = 0; j < n; j++) {
          fillArray(hash[1][j]->root, nodesi, j, &k);
        }
        k=0;
        for (size_t j = 0; j < n; j++) {
          fillArray(hash[BLOCK_SIZE(id, nprocs, n)+1][j]->root, nodesf, j, &k);
        }



	      /** Send and Receive Information */
	      MPI_Send(&sizef, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);//send size up
        MPI_Send(&sizei, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);//send size down
        MPI_Send(&nodesf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);//send nodes up
        MPI_Send(&nodesi, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);//send nodes down



        MPI_Recv(&rcvdsizef, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//receive size up
        MPI_Recv(&rcvdsizei, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//receive size down

        rcvdnodesf = (int*)malloc(sizeof(int)*rcvdsizef*2);
        rcvdnodesi = (int*)malloc(sizeof(int)*rcvdsizei*2);

        MPI_Recv(&rcvdnodesf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//receive nodes up
        MPI_Recv(&rcvdnodesi, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//receive nodes down

        

        MPI_Barrier();

        /** Compute next generation */
        nextGen(hash, insert, delete, n, id, nprocs);
    }

    /** Free Linked Lists */
    //free(insert[i]);
    //free(delete[i]);

    /** Free Tree */
    //freeTree(hash, n);

    MPI_Finalize();

    /** Print tree to stdout */
    //printTree(hash, n);



    return 0;
}
