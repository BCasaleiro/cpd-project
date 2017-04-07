#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "octree.h"
#include <omp.h>

extern omp_lock_t lck_a;
extern omp_lock_t lck_octree;

void print_octree(octree * o,octree_node * o_n){
    int max_depth = o->max_depth;
    if(o_n->depth == max_depth-1){
       int i ;
        for(i=0;i<o_n->depth;i++){
            printf("  ");
        }
        printf("d: %d\tl: %d\n",o_n->depth,o_n->location);
        for(i=0;i<8;i++){
            if(o_n->leaf_children[i]!=NULL){
                int j = 0;
                for(j=0;j<o_n->depth+1;j++){
                    printf("  ");
                }
                printf("d: %d\tl: %d life:%d\n",o_n->depth+1,i,o_n->leaf_children[i]->lives);
                
            
            }
        }
    
    }
    else{
        
        int j = 0;
        for(j=0;j<o_n->depth;j++){
            printf("  ");
        }
        printf("d: %d\tl: %d\n",o_n->depth,o_n->location);
       for(j=0;j<8;j++){
        if(o_n->children[j]!=NULL){
            print_octree(o,o_n->children[j]);
        }   
       }
    }


}


void delete_inner_node(octree_node * o_n, char index){
    
    free(o_n->children[index]->children);
    free(o_n->children[index]->leaf_children);
    free(o_n->children[index]);
    o_n->children[index]=NULL;
    o_n->live_children--;
    if((o_n->live_children==0)&& (o_n->parent != NULL)){ 
        delete_inner_node(o_n->parent, o_n->location);
    }
}
void delete_leaf_node(octree *o, octree_node * o_n,int i){


    free(o_n->leaf_children[i]);
    o_n->leaf_children[i]=NULL;
    o_n->live_children--;
    o->leaf_population--;
    if(o_n->live_children == 0){
        delete_inner_node(o_n->parent, o_n->location);
    }

}
void free_octree(octree* o,octree_node * o_n ){
 
    int max_depth = o->max_depth;
    if(o_n->depth == max_depth-1){
        int i;
        for(i=0;i<8;i++){
            if((o_n->leaf_children[i]!=NULL)){//&&(o_n->leaf_children[i]->lives == 1)){
                free(o_n->leaf_children[i]);
                o_n->live_children--;

            }
        }

        if(o_n->live_children == 0){
            delete_inner_node(o_n->parent, o_n->location);
        }
    }
    else{
        if(o_n->children[0]!=NULL){
            free_octree(o,o_n->children[0]);
        }
        if(o_n->children[1]!=NULL){
            free_octree(o,o_n->children[1]);
        }
        if(o_n->children[2]!=NULL){
            free_octree(o,o_n->children[2]);
        }
        if(o_n->children[3]!=NULL){
            free_octree(o,o_n->children[3]);
        }
        if(o_n->children[4]!=NULL){
            free_octree(o,o_n->children[4]);
        }
        if(o_n->children[5]!=NULL){
            free_octree(o,o_n->children[5]);
        }
        if(o_n->children[6]!=NULL){
            free_octree(o,o_n->children[6]);
        }
        if(o_n->children[7]!=NULL){
            free_octree(o,o_n->children[7]);
        }
    }
   


}

void update_octree(octree*o, octree_node * o_n){

    
    int max_depth = o->max_depth;
    if(o_n->depth == max_depth-1){
        int i;
        for(i=0;i<8;i++){
            if((o_n->leaf_children[i]!=NULL)){//&&(o_n->leaf_children[i]->lives == 1)){
                if(o_n->leaf_children[i]->lives == 1){
                    int aux = o_n->leaf_children[i]->n;
                    if((aux<2)||(aux>4)){
                        delete_leaf_node(o,o_n,i);
                    }else{
                        o_n->leaf_children[i]->n=0;
                    }
                }
                else{
                    int aux = o_n->leaf_children[i]->n;
                    if((aux==2)||(aux==3)){
                        o_n->leaf_children[i]->n=0;
                        o_n->leaf_children[i]->lives=1;

                    }else{
                        delete_leaf_node(o,o_n,i);
                    }
                }
            }
        }    
    }
    else{
        if(o_n->children[0]!=NULL){
            update_octree(o,o_n->children[0]);
        }
        if(o_n->children[1]!=NULL){
            update_octree(o,o_n->children[1]);
        }
        if(o_n->children[2]!=NULL){
            update_octree(o,o_n->children[2]);
        }
        if(o_n->children[3]!=NULL){
            update_octree(o,o_n->children[3]);
        }
        if(o_n->children[4]!=NULL){
            update_octree(o,o_n->children[4]);
        }
        if(o_n->children[5]!=NULL){
            update_octree(o,o_n->children[5]);
        }
        if(o_n->children[6]!=NULL){
            update_octree(o,o_n->children[6]);
        }
        if(o_n->children[7]!=NULL){
            update_octree(o,o_n->children[7]);
        }
    }

}


void get_neighbor_coords(ccc coordinates, int ** neighbors, int size){
    int i = 0;
    for(i=0;i<6;i++){
        int j;
            neighbors[i][0]=coordinates.x;
            neighbors[i][1]=coordinates.y;
            neighbors[i][2]=coordinates.z;
    }
    neighbors[0][0]=(neighbors[0][0]+1)%size;
    neighbors[1][0]--;
    if(neighbors[1][0]<0){
        neighbors[1][0]=size-1;
    }
    neighbors[2][1]=(neighbors[2][1]+1)%size;
    neighbors[3][1]--;
    if(neighbors[3][1]<0){
        neighbors[3][1]=size-1;
    }
    neighbors[4][2]=(neighbors[4][2]+1)%size;
    neighbors[5][2]--;
    if(neighbors[5][2]<0){
        neighbors[5][2]=size-1;
    }
}

int mk_neighbor(octree* o,octree_node * o_n,int *coordinates,int *n_coordinates){

    int size = o->size;
    int depth = o_n->depth;
    int max_depth = o->max_depth;

    int middle = (1<<(max_depth-depth))/2;
    char location = find_octant((o_n),coordinates,n_coordinates, middle);   


    if(depth == max_depth-1){  //next node is a leaf_node
        if(o_n->leaf_children[location]==NULL){
            omp_set_lock(&lck_a);
            o_n->leaf_children[location]=malloc(sizeof(*o_n->leaf_children[location]));
            omp_unset_lock(&lck_a);
            o_n->leaf_children[location]->location = location;
            o_n->leaf_children[location]->n = 1;
            o_n->leaf_children[location]->lives = 0;
            
            o_n->live_children++;

            omp_set_lock(&lck_octree);
            
            o->leaf_population++;
            
            omp_unset_lock(&lck_octree);
            return 0;

        }else{

            if(o_n->leaf_children[location]->lives == 0){
                o_n->leaf_children[location]->n++;
                return 0;
            }else{
                return 1;
            }
        }
    }else{                      //next node is an inner node of the octree
        if(o_n->children[location]== NULL){
            omp_set_lock(&lck_a);
            o_n->children[location] = malloc(sizeof(*o_n->children[location]));
            omp_unset_lock(&lck_a);
            o_n->children[location]->depth = o_n->depth+1;
            o_n->children[location]->live_children = 0;
            
            omp_set_lock(&lck_a);
            o_n->children[location]->children = malloc(8*sizeof(*o_n->children[location]->children));
            omp_unset_lock(&lck_a);
            o_n->children[location]->children[0]=NULL;
            o_n->children[location]->children[1]=NULL;
            o_n->children[location]->children[2]=NULL;
            o_n->children[location]->children[3]=NULL;
            o_n->children[location]->children[4]=NULL;
            o_n->children[location]->children[5]=NULL;
            o_n->children[location]->children[6]=NULL;
            o_n->children[location]->children[7]=NULL;
         
            omp_set_lock(&lck_a);
            o_n->children[location]->leaf_children = malloc(8*sizeof(*o_n->children[location]->leaf_children));
            omp_unset_lock(&lck_a);
            o_n->children[location]->leaf_children[0]=NULL;
            o_n->children[location]->leaf_children[1]=NULL;
            o_n->children[location]->leaf_children[2]=NULL;
            o_n->children[location]->leaf_children[3]=NULL;
            o_n->children[location]->leaf_children[4]=NULL;
            o_n->children[location]->leaf_children[5]=NULL;
            o_n->children[location]->leaf_children[6]=NULL;
            o_n->children[location]->leaf_children[7]=NULL;
            
            o_n->children[location]->location = location;
            o_n->children[location]->parent = o_n;
            o_n->live_children = o_n->live_children +1;
            char aux_location = location;
            n_coordinates[2]=n_coordinates[2]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[1]=n_coordinates[1]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[0]=n_coordinates[0]+middle * (location & 0x1);
            mk_neighbor(o,o_n->children[aux_location], coordinates, n_coordinates);
        }else{
            char aux_location = location;
            n_coordinates[2]=n_coordinates[2]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[1]=n_coordinates[1]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[0]=n_coordinates[0]+middle * (location & 0x1);
            mk_neighbor(o, o_n->children[aux_location], coordinates, n_coordinates);
        
            
        }
    }

}
void mk_neighborhood(octree * o, octree_node*o_n, char * aux_location){
    int max_depth = o->max_depth;

    octree_node* aux = o_n;
    aux_location[o_n->depth-1]=o_n->location;
    
    //octree_node* aux = o_n;
    //aux_location[aux->depth-1]=aux->location;
    if(o_n->depth == max_depth-1){
        
        omp_set_lock(&lck_a);
        int **neighbors = malloc(sizeof(int*)*6);
        omp_unset_lock(&lck_a);
        int i =0;
        int j = 0;
        omp_set_lock(&lck_a);
        for(i=0;i<6;i++){
            neighbors[i]=malloc(sizeof(int)*3);
        }
        omp_unset_lock(&lck_a);
        for(i=0;i<8;i++){
            if((o_n->leaf_children[i]!=NULL)&&(o_n->leaf_children[i]->lives == 1)){
                aux_location[o_n->depth]=o_n->leaf_children[i]->location;
                ccc cord;
                coordinates_from_location(aux_location, max_depth,&cord);
                omp_set_lock(&lck_a);
                get_neighbor_coords(cord, neighbors, o->size);
                omp_unset_lock(&lck_a);
                int neighbor_count=0;
                int ii;
                for( ii=0;ii<6;ii++){
                    
                    int n_coordinates[3] = {0,0,0};
                    neighbor_count += mk_neighbor(o,o->root,neighbors[ii],n_coordinates);
                        
                }
                o_n->leaf_children[i]->n = neighbor_count;
            }
        }    
        omp_set_lock(&lck_a);
        for(i=0;i<6;i++){
            free(neighbors[i]);
        }
        free(neighbors);
        omp_unset_lock(&lck_a);
    }
    else{
        if(o_n->children[0]!=NULL){
            mk_neighborhood(o,o_n->children[0],aux_location);
        }
        if(o_n->children[1]!=NULL){
            mk_neighborhood(o,o_n->children[1],aux_location);
        }
        if(o_n->children[2]!=NULL){
            mk_neighborhood(o,o_n->children[2],aux_location);
        }
        if(o_n->children[3]!=NULL){
            mk_neighborhood(o,o_n->children[3],aux_location);
        }
        if(o_n->children[4]!=NULL){
            mk_neighborhood(o,o_n->children[4],aux_location);
        }
        if(o_n->children[5]!=NULL){
            mk_neighborhood(o,o_n->children[5],aux_location);
        }
        if(o_n->children[6]!=NULL){
            mk_neighborhood(o,o_n->children[6],aux_location);
        }
        if(o_n->children[7]!=NULL){
            mk_neighborhood(o,o_n->children[7],aux_location);
        }
    
    }

}


    
    



void coordinates_from_location(char * location,int max_depth,ccc * coordinates){
        int j;
        coordinates->x=0;
        coordinates->y=0;
        coordinates->z=0;
        int size=0;
        for(j=0;j<max_depth;j++){
            size = 1<<((max_depth-1)-j);
            
            if((location[j] & 0x1) == 1){
                coordinates->z=coordinates->z+((1<<(max_depth-1-j)));
            }
            if((location[j] & 0x2 )== 2){
                coordinates->y=coordinates->y+((1<<(max_depth-1-j)));
            }
            if((location[j] & 0x4) == 4){
                coordinates->x=coordinates->x+((1<<(max_depth-1-j)));
            }
        }


}

int larger_coordinate(const void *c, const void * d){
    coord * a = (coord *) c;

    coord * b = (coord *)d;


    if(a->coords[0]<b->coords[0])return 0;
    if(a->coords[0]>b->coords[0])return 1;

    if(a->coords[1]<b->coords[1])return 0;
    if(a->coords[1]>b->coords[1])return 1;
    
    if(a->coords[2]<b->coords[2])return 0;
    if(a->coords[2]>b->coords[2])return 1;
}


coord* get_leaf_nodes_locations(octree * o, octree_node*o_n, char * aux_location){
    int max_depth = o->max_depth;
    octree_node* aux = o_n;
    aux_location[aux->depth-1]=aux->location;
    if(o_n->depth == max_depth-1){
        int i;
        for(i=0;i<8;i++){
            if((o_n->leaf_children[i]!=NULL)){//&&(o_n->leaf_children[i]->lives == 1)){
                aux_location[o_n->depth]=o_n->leaf_children[i]->location;
                ccc coord;
                coordinates_from_location(aux_location, max_depth,&coord);
                //coord * new_coord = malloc(sizeof(*new_coord));
                
                leaf_nodes_coords[index1].coords[0]=coord.x;
                leaf_nodes_coords[index1].coords[1]=coord.y;
                leaf_nodes_coords[index1].coords[2]=coord.z;
                index1++;
              //  printf("%d %d %d \t lives: %d \t neighbors:%d\t\n",coordinates[0],coordinates[1],coordinates[2],o_n->leaf_children[i]->lives, o_n->leaf_children[i]->n);
            }
        }    
    }
    else{
        if(o_n->children[0]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[0],aux_location);
        }
        if(o_n->children[1]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[1],aux_location);
        }
        if(o_n->children[2]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[2],aux_location);
        }
        if(o_n->children[3]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[3],aux_location);
        }
        if(o_n->children[4]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[4],aux_location);
        }
        if(o_n->children[5]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[5],aux_location);
        }
        if(o_n->children[6]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[6],aux_location);
        }
        if(o_n->children[7]!=NULL){
            get_leaf_nodes_locations(o,o_n->children[7],aux_location);
        }
    
    }

}


int find_max_depth(int size){

    int depth = 1;
    int size_aux = 1;
    while(size_aux<size){
        size_aux = size_aux*2;
        depth++;
    //    printf("%d ", size_aux);
    }
    return depth;
}
octree * read_input(FILE * fp){
    octree * octree;
    octree = malloc(sizeof(*octree));
    size_t len = 0; char* line /*= malloc(sizeof(char)*24)*/; int size=0;int coordinates[3]={0,0,0};
    //memset(line,0,24 * sizeof(char));
    getline(&line, &len, fp);    
    sscanf(line,"%d",&size);
    //printf("%d\n",size);
    octree->size = size;
    octree->leaf_population = 0;
    int bla=find_max_depth(size);
    octree->max_depth = bla;
    octree->root = malloc(sizeof(octree_node));
    if (octree->root != NULL){
        octree->root->depth=1;
        octree->root->location=8;
        octree->root->live_children=0;
        octree->root->children=malloc(8*sizeof(*octree->root->children));
        octree->root->children[0]=NULL;
        octree->root->children[1]=NULL;
        octree->root->children[2]=NULL;
        octree->root->children[3]=NULL;
        octree->root->children[4]=NULL;
        octree->root->children[5]=NULL;
        octree->root->children[6]=NULL;
        octree->root->children[7]=NULL;
        octree->root->leaf_children=malloc(8*sizeof(*octree->root->leaf_children));
        octree->root->leaf_children[0]=NULL;
        octree->root->leaf_children[1]=NULL;
        octree->root->leaf_children[2]=NULL;
        octree->root->leaf_children[3]=NULL;
        octree->root->leaf_children[4]=NULL;
        octree->root->leaf_children[5]=NULL;
        octree->root->leaf_children[6]=NULL;
        octree->root->leaf_children[7]=NULL;
        octree->root->parent = NULL;
    }
    else{
        exit(1);
    }
    // Populating the Octree
    int n_coordinates[3];
    memset(line,0,24 * sizeof(char));
    while(getline(&line,&len,fp)!= -1){
        sscanf(line,"%d %d %d",&coordinates[0], &coordinates[1], &coordinates[2]);
        int n_coordinates[3] = {0,0,0};
        //printf("%d %d %d\n", coordinates[0],coordinates[1],coordinates[2]);
        insert_in_octree(octree,(octree->root),coordinates, n_coordinates);

        memset(line,0,24 * sizeof(char));
    }
    fclose(fp);
    free(line);
    return octree;
}


void insert_in_octree(octree * o, octree_node * o_n, int coordinates[3], int n_coordinates[3]){

    int size = o->size;
    int depth = o_n->depth;
    int max_depth = o->max_depth;

    int middle = (1<<(max_depth-depth))/2;
    char location = find_octant((o_n),coordinates,n_coordinates, middle);   


    if(depth == max_depth-1){  //next node is a leaf_node
        if(o_n->leaf_children[location]==NULL){
            o_n->leaf_children[location]=malloc(sizeof(*o_n->leaf_children[location]));
            o_n->leaf_children[location]->location = location;
            o_n->leaf_children[location]->n = 0;
            o_n->leaf_children[location]->lives = 1;
            o_n->live_children++;
            o->leaf_population++;
        }
    }else{                      //next node is an inner node of the octree
        if(o_n->children[location]== NULL){
            o_n->children[location] = (octree_node *)malloc(sizeof(*o_n->children[location]));
            o_n->children[location]->depth = o_n->depth+1;
            o_n->children[location]->live_children = 0;
            o_n->children[location]->children = malloc(8*sizeof(*o_n->children[location]->children));
            o_n->children[location]->children[0]=NULL;
            o_n->children[location]->children[1]=NULL;
            o_n->children[location]->children[2]=NULL;
            o_n->children[location]->children[3]=NULL;
            o_n->children[location]->children[4]=NULL;
            o_n->children[location]->children[5]=NULL;
            o_n->children[location]->children[6]=NULL;
            o_n->children[location]->children[7]=NULL;
            o_n->children[location]->leaf_children = malloc(8*sizeof(*o_n->children[location]->leaf_children));
            o_n->children[location]->leaf_children[0]=NULL;
            o_n->children[location]->leaf_children[1]=NULL;
            o_n->children[location]->leaf_children[2]=NULL;
            o_n->children[location]->leaf_children[3]=NULL;
            o_n->children[location]->leaf_children[4]=NULL;
            o_n->children[location]->leaf_children[5]=NULL;
            o_n->children[location]->leaf_children[6]=NULL;
            o_n->children[location]->leaf_children[7]=NULL;
            o_n->children[location]->location = location;
            o_n->children[location]->parent = o_n;
            o_n->live_children = o_n->live_children +1;
            char aux_location = location;
            n_coordinates[2]=n_coordinates[2]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[1]=n_coordinates[1]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[0]=n_coordinates[0]+middle * (location & 0x1);
            insert_in_octree(o,o_n->children[aux_location], coordinates, n_coordinates);
        }else{
            char aux_location = location;
            n_coordinates[2]=n_coordinates[2]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[1]=n_coordinates[1]+middle * (location & 0x1);
            location = location>>1;
            n_coordinates[0]=n_coordinates[0]+middle * (location & 0x1);
            insert_in_octree(o, o_n->children[aux_location], coordinates, n_coordinates);
        
            
        }
    }
}

char find_octant(octree_node * o_n,int coordinates[3],int n_coordinates[3], int middle){
    char location = 0;
    // x coordinate
    if (coordinates[0] < n_coordinates[0]+middle){
        location = location | 0x0;
    }else{
        location = location | 0x4;
    }
    // y coordinate
    if (coordinates[1] < n_coordinates[1]+middle){
        location = location | 0x0;
    }else{
        location = location | 0x2;
    }
    // z coordinate
    if (coordinates[2] < n_coordinates[2] + middle){
        location = location | 0x0;
    }else{
        location = location | 0x1;
    }
    //printf("%d \t",location);
    return location;

}
