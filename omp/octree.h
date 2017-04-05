

/*
 * char location: 
 *          0xx -> bottom   (z axis)
 *          1xx -> top      (z axis)
 *          x0x -> right    (x axis)
 *          x1x -> left     (x axis)
 *          xx0 -> front    (y axis)   
 *          xx1 -> back     (y axis)
 */
typedef struct coord{
    short coords[3];
}coord;

coord*leaf_nodes_coords;
int index1;
typedef struct octree_node{
    uint8_t depth;
    uint8_t live_children;
    struct octree_node **children;
    struct octree_leaf_node **leaf_children;
    char location;
    struct octree_node * parent;

}octree_node;

typedef struct octree_leaf_node{
    char location;
    uint8_t n;
    uint8_t lives;
}octree_leaf_node;

typedef struct octree{
    int size;
    int leaf_population; 
    octree_node * root;
    int max_depth;
}octree;

octree * read_input(FILE * fp);
int find_max_depth(int size);
char find_octant(octree_node * o_n, int coordinates[3], int n_coordinates[3], int middle);
void print_children(octree_node * o_n, int max_depth);
void insert_in_octree(octree * o, octree_node * o_n, int coordinates[3], int n_coordinates[3]);
int larger_coordinate(const void * c, const void *d);
void print_locations(octree * o, coord* locations);
int * coordinates_from_location(char * location, int max_depth);
coord *  get_leaf_nodes_locations(octree * o, octree_node*o_n, char * aux_location);
void free_octree(octree*o, octree_node* o_n);
void update_octree(octree*o,octree_node *o_n);
void mk_neighborhood(octree * o, octree_node*o_n, char * aux_location);
void print_octree(octree *o, octree_node * o_n);
