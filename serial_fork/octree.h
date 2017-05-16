

/*
 * char location: 
 *          0xx -> bottom   (z axis)
 *          1xx -> top      (z axis)
 *          x0x -> right    (x axis)
 *          x1x -> left     (x axis)
 *          xx0 -> front    (y axis)   
 *          xx1 -> back     (y axis)
 */

typedef struct octree_node{
    uint8_t depth;
    
    struct octree_node * parent;
    char location;
    uint8_t live_children;
}octree_node;

typedef struct octree{
    int size;
    octree_node * nodes;
    int max_depth;
}octree;

octree * read_input(FILE * fp);
int find_max_depth(int size);
char find_octant(octree_node * o_n, int coordinates[3], float middle);

void insert_in_octree(octree * o, octree_node * o_n, int coordinates[3]);
