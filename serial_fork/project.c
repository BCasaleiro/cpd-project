#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "octree.h"

int main(int argc, char* argv[]){

    if(argc<3){
        printf("Not enough args: requires 2\n");
        exit -1;
    }

    FILE * input = fopen(argv[1],"r");
    
    if (input == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }

    octree  * octree = read_input(input);
    printf("max depth: %d\n",octree->max_depth);
    
}
