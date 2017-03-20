#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "octree.h"


int find_max_depth(int size){

    int depth = 0;
    int size_aux = 1;
    while(size_aux<size){
        size_aux = size_aux*2;
        depth++;
        printf("%d ", size_aux);
    }
    return depth;
}


octree * read_input(FILE * fp){

    octree * octree = malloc(sizeof(octree));
    size_t len = 0; char * line; int size=0;int coordinates[3];
    
    getline(&line, &len, fp);    
    sscanf(line,"%d",&size);
    printf("%d\n",size);
    octree->size = size;
    
    octree->max_depth = find_max_depth(size);
    octree->nodes = malloc(sizeof(octree_node));
    
    if (octree->nodes != NULL){
        
    
    octree->nodes[0].parent = NULL;
    octree->nodes[0].depth=1;
    octree->nodes[0].location=8;
    octree->nodes[0].live_children=0;
    }
    else{
        exit(1);
    }
    while(getline(&line,&len,fp)!= -1){
        printf("%s\n",line);
        sscanf(line,"%d %d %d",&coordinates[0], &coordinates[1], &coordinates[2]);
        insert_in_octree(octree,&(octree->nodes[0]),coordinates);
    }
    return octree;
}
char find_octant(octree_node * o_n,int coordinates[3], float middle){
    char location = 0;
    // x coordinate
    if (coordinates[0] <= middle){
        location = location | 0x0;
    }else{
        location = location | 0x2;
    }
    // y coordinate
    if (coordinates[1] <= middle){
        location = location | 0x0;
    }else{
        location = location | 0x1;
    }
    // z coordinate
    if (coordinates[2] <= middle){
        location = location | 0x0;
    }else{
        location = location | 0x4;
    }

    return location;

}
void insert_in_octree(octree * o, octree_node * o_n, int coordinates[3]){

    int size = o->size;
    int depth = o_n->depth;
    float middle = (size/depth)/2;
    int max_depth = o->max_depth;
    char location = find_octant(&(o->nodes[0]),coordinates, middle);   
    printf("%x\n", location);

}
