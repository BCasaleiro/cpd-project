#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "octree.h"

coord * leaf_nodes_coords = NULL;
int index1 =0;

int main(int argc, char* argv[]){

    if(argc<3){
        printf("Not enough args: requires 2\n");
        exit(1);
    }

    FILE * input = fopen(argv[1],"r");
    
    if (input == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }
    int seasons = atoi(argv[2]);

    octree  * octree = read_input(input);
    printf("max depth: %d\n",octree->max_depth);
    coord ** locations = malloc(sizeof(coord*));
    *locations=NULL;
    char aux_location[octree->max_depth];
    aux_location[0]=octree->root->location;
    int i = 0;
    while(i<seasons){
        mk_neighborhood(octree, octree->root,aux_location);
        update_octree(octree, octree->root);
        i++;
        printf("%d\n",i);
    }

    //printf("into get_leaf_nodes_locations\n");
    leaf_nodes_coords = malloc(octree->leaf_population*sizeof(coord));
    get_leaf_nodes_locations(octree, octree->root ,aux_location);
    qsort(leaf_nodes_coords,octree->leaf_population, sizeof(coord), larger_coordinate);
    index1=0;
    FILE * output = fopen("output","w");
    while(index1<octree->leaf_population){
     //   printf("badjoras\n");
        fprintf(output,"%d %d %d\n", leaf_nodes_coords[index1].coords[0],leaf_nodes_coords[index1].coords[1],leaf_nodes_coords[index1].coords[2]);
        index1++;
 
    }
    fclose(output);
    free_octree(octree, octree->root);    
}


