#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "octree.h"

coord * leaf_nodes_coords = NULL;
int index1 =0;

int one_half[4]={0,3,6,5};
int other_half[4]={1,2,4,7};

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
    char aux_location[octree->max_depth];
    aux_location[0]=octree->root->location;
    int i = 0;
    int j = 0;
    while(i<seasons){

        for(j=0;j<4;j++){
            if(octree->root->children[one_half[j]]!=NULL){
                octree_node * aux = octree->root->children[one_half[j]];
                aux_location[1]=aux->location;
                mk_neighborhood(octree, aux,aux_location);
                    
            }
        
        }

        for(j=0;j<4;j++){
            if(octree->root->children[other_half[j]]!=NULL){
                octree_node * aux = octree->root->children[other_half[j]];
                aux_location[1]=aux->location;
                mk_neighborhood(octree, aux,aux_location);
                    
            }
        
        }
       //mk_neighborhood(octree, octree->root,aux_location);
        //update_octree(octree, octree->root);
        for(j=0;j<4;j++){
            if(octree->root->children[one_half[j]]!=NULL){
                octree_node * aux = octree->root->children[one_half[j]];
                update_octree(octree, aux);
            }
        
        }
        for(j=0;j<4;j++){
            if(octree->root->children[other_half[j]]!=NULL){
                octree_node * aux = octree->root->children[other_half[j]];
                update_octree(octree, aux);
            }
        
        }
        i++;
        printf("%d    leafs: %d\n",i,octree->leaf_population );

//        print_octree(octree,octree->root);
    }

    leaf_nodes_coords = malloc(octree->leaf_population*sizeof(coord));
    get_leaf_nodes_locations(octree, octree->root ,aux_location);
    qsort(leaf_nodes_coords,octree->leaf_population, sizeof(coord), larger_coordinate);
    index1=0;

    FILE * output = fopen("output","w");

    while(index1<octree->leaf_population){
        fprintf(output,"%d %d %d\n", leaf_nodes_coords[index1].coords[0],leaf_nodes_coords[index1].coords[1],leaf_nodes_coords[index1].coords[2]);
        
        index1++;
 
    }

    free((leaf_nodes_coords));
    fclose(output);
    free_octree(octree, octree->root);    
    free(octree->root->children);
    free(octree->root->leaf_children);
    free(octree->root);
    free(octree);
}

