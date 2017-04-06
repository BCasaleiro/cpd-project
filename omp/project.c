#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "octree.h"
#include <omp.h>


coord * leaf_nodes_coords = NULL;
int index1 =0;
omp_lock_t lck_a;

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
    
    int i = 0;
    int j = 0;
    int k = 0;
    omp_init_lock(&lck_a);
    while(i<seasons){
        for(j=0;j<8;j++){   //    j indexes root's grand_children

            octree_node * aux; 
# pragma omp parallel for private(k,aux),firstprivate(j)
            for(k=0;k<8;k++){   // k indexes root's children    
                char aux_location[octree->max_depth];
                aux_location[0]=octree->root->location;
                if((octree->root->children[k]!=NULL) && (octree->root->children[k]->children[j]!=NULL)){
            
                    aux = octree->root->children[k]->children[j];
        //        printf("root->%d->%d\n",k,j);
                    aux_location[1]= k;
                    aux_location[2]= j;
                    mk_neighborhood(octree, aux, aux_location);
                }
            }
        }
        //mk_neighborhood(octree, octree->root,aux_location);
        update_octree(octree, octree->root);
        i++;
        printf("%d    leafs: %d\n",i,octree->leaf_population );

//        print_octree(octree,octree->root);
    }

    char aux_location[octree->max_depth];
    aux_location[0]=octree->root->location;
    leaf_nodes_coords = malloc(octree->leaf_population*sizeof(coord));
    get_leaf_nodes_locations(octree, octree->root ,aux_location);
    qsort(leaf_nodes_coords,octree->leaf_population, sizeof(coord), larger_coordinate);
    index1=0;

    FILE * output = fopen("output","w");

    while(index1<octree->leaf_population){
        fprintf(output,"%d %d %d\n", leaf_nodes_coords[index1].coords[0],leaf_nodes_coords[index1].coords[1],leaf_nodes_coords[index1].coords[2]);
        
        index1++;
 
    }

    fclose(output);
    free((leaf_nodes_coords));
    free_octree(octree, octree->root);    
    free(octree->root->children);
    free(octree->root->leaf_children);
    free(octree->root);
    free(octree);
}


