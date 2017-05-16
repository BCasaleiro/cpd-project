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
    int i, k, n, j;
    int id, nprocs;
    Row *insert, *delete;
	MPI_Request request_up;
	MPI_Request request_down;
	MPI_Status status_up;
	MPI_Status status_down;
	MPI_Request request_vup;
	MPI_Request request_vdown;
	MPI_Status status_vup;
	MPI_Status status_vdown;


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
    //printTree(hash, n, id, nprocs);

    insert=(Row*)malloc(sizeof(Row));
    delete=(Row*)malloc(sizeof(Row));

    int sizef, sizei, aux,recv_size_f, recv_size_i;
    sizef=sizei=aux=0;

    int * nodesf;
	int * nodesi;
    int * recv_nodesi;
    int * recv_nodesf;

    for (i = 0; i < k; i++) {
        sizef=sizei=0;
        aux=0;

        //get number of nodes
        for(j=0; j<n; j++){
          sizei = (*hash)[1][j]->size + sizei;
        }

        for(j=0; j<n; j++){
          sizef = (*hash)[BLOCK_SIZE(id,nprocs,n)][j]->size + sizef;
        }

        //alloc memory for sending arrays
        nodesi = (int*)malloc(sizei*sizeof(int)*2);
        nodesf = (int*)malloc(sizef*sizeof(int)*2);

        //fill the arrays with yz nodes
        for (j = 0; j < n; j++) {
          fillArray((*hash)[1][j]->root, nodesi, j, &aux);
        }

        aux=0;
        for ( j = 0; j < n; j++) {
          fillArray((*hash)[BLOCK_SIZE(id,nprocs,n)][j]->root, nodesf, j, &aux);
        }


        //receive count of members of array Xi-1
        if ( BLOCK_LOW(id,nprocs,n) > 0 ) {
            MPI_Irecv(&recv_size_i, 1, MPI_INT, BLOCK_OWNER(BLOCK_LOW(id,nprocs,n)-1,nprocs,n),LOWER_COUNT,MPI_COMM_WORLD,&request_down);
            // printf("[%d] Ready to receive from %d\n", id, BLOCK_OWNER(BLOCK_LOW(id,nprocs,n)-1,nprocs,n));
        } else {
            MPI_Irecv(&recv_size_i, 1, MPI_INT, BLOCK_OWNER(n-1,nprocs,n),LOWER_COUNT,MPI_COMM_WORLD,&request_down);
            // printf("[%d] Ready to receive from %d\n", id, BLOCK_OWNER(n-1,nprocs,n));
        }

        //receive count of members of array Xf+1
	    MPI_Irecv(&recv_size_f, 1, MPI_INT, BLOCK_OWNER(BLOCK_LOW((id+1)%nprocs,nprocs,n),nprocs,n),UPPER_COUNT,MPI_COMM_WORLD,&request_up);
        // printf("[%d] Ready to receive from %d\n", id , BLOCK_OWNER(BLOCK_LOW((id+1)%nprocs,nprocs,n),nprocs,n));

        //send xi count
        if ( BLOCK_LOW(id,nprocs,n) > 0 ) {
            MPI_Send(&sizei, 1, MPI_INT, BLOCK_OWNER(BLOCK_LOW(id,nprocs,n)-1,nprocs,n),UPPER_COUNT,MPI_COMM_WORLD);
        } else {
            MPI_Send(&sizei, 1, MPI_INT, BLOCK_OWNER(n-1,nprocs,n),UPPER_COUNT,MPI_COMM_WORLD);
        }

        //send xf count
        MPI_Send(&sizef, 1, MPI_INT, BLOCK_OWNER(BLOCK_LOW((id+1)%nprocs,nprocs,n),nprocs,n),LOWER_COUNT,MPI_COMM_WORLD);

        MPI_Wait(&request_down,&status_down);
        MPI_Wait(&request_up,&status_up);

        // printf("[%d] sent lower_count: %d; sent  upper_count: %d\n",id, sizei,sizef);
        // printf("[%d] received lower_count: %d; received upper_count: %d\n",id, recv_size_i,recv_size_f);

	recv_nodesi = (int*) malloc((2*recv_size_i) * sizeof(int));
	recv_nodesf = (int*) malloc((2*recv_size_f) * sizeof(int));

        //receive array Xi-1
        if ( BLOCK_LOW(id,nprocs,n) > 0 ) {
            MPI_Irecv(recv_nodesi, (2*recv_size_i), MPI_INT, BLOCK_OWNER(BLOCK_LOW(id,nprocs,n)-1,nprocs,n),LOWER_VECTOR,MPI_COMM_WORLD,&request_vdown);
        } else {
            MPI_Irecv(recv_nodesi, (2*recv_size_i), MPI_INT, BLOCK_OWNER(n-1,nprocs,n),LOWER_VECTOR,MPI_COMM_WORLD,&request_vdown);
        }
        //receive array Xf+1
        MPI_Irecv(recv_nodesf, (2*recv_size_f), MPI_INT, BLOCK_OWNER(BLOCK_LOW((id+1)%nprocs,nprocs,n),nprocs,n),UPPER_VECTOR,MPI_COMM_WORLD,&request_vup);

        //send xi
        if ( BLOCK_LOW(id,nprocs,n) > 0 ) {
            MPI_Send(nodesi, (2*sizei), MPI_INT, BLOCK_OWNER(BLOCK_LOW(id,nprocs,n)-1,nprocs,n),UPPER_VECTOR,MPI_COMM_WORLD);
        } else {
            MPI_Send(nodesi, (2*sizei), MPI_INT, BLOCK_OWNER(n-1,nprocs,n),UPPER_VECTOR,MPI_COMM_WORLD);
        }
        //send xf
        MPI_Send(nodesf, (2*sizef), MPI_INT, BLOCK_OWNER(BLOCK_LOW((id+1)%nprocs,nprocs,n),nprocs,n),LOWER_VECTOR,MPI_COMM_WORLD);

	      MPI_Wait(&request_vdown,&status_vdown);
        MPI_Wait(&request_vup,&status_vup);

	/*
	size_t o;
	printf("[%d] Sent xi [ ", id);
	for (o = 0; o < 2*sizei; o++) {
		printf("%d ", nodesi[o]);
	}
	printf("]\n");

	printf("[%d] Sent xf [ ", id);
	for (o = 0; o < 2*sizef; o++) {
		printf("%d ", nodesf[o]);
	}
	printf("]\n");

	printf("[%d] Received xi-1 [ ", id);
	for (o = 0; o < 2*recv_size_i; o++) {
		printf("%d ", recv_nodesi[o]);
	}
	printf("]\n");

	printf("[%d] Received xf+1 [ ", id);
	for (o = 0; o < 2*recv_size_f; o++) {
		printf("%d ", recv_nodesf[o]);
	}
	printf("]\n");
	*/

        for(j=0;j<2*recv_size_i;j++){
            (*hash)[0][recv_nodesi[j]]->root = insertTree(recv_nodesi[j+1], (*hash)[0][recv_nodesi[j]]->root, &(*hash)[0][recv_nodesi[j]]->size);
            j++;
        }

        for(j=0;j<2*recv_size_f;j++){
            (*hash)[BLOCK_SIZE(id,nprocs,n)+1][recv_nodesf[j]]->root = insertTree(recv_nodesf[j+1], (*hash)[BLOCK_SIZE(id,nprocs,n)+1][recv_nodesf[j]]->root, &(*hash)[BLOCK_SIZE(id,nprocs,n)+1][recv_nodesf[j]]->size);
		j++;
        }

	//printTree(hash, n, id, nprocs);

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
