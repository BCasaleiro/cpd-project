#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N_BRANCHS 8
#define N_NEIGHBORS 6

typedef struct cell cell;
typedef struct cell {
    cell** neighbors[6];
    bool checked[2];
    bool alive[2];
} cell;

typedef struct branch branch;
typedef struct branch {
    branch* children;
    cell** cells;
} branch;

void cleanup(branch* b) {
    if ( b->children != NULL ) {
        for (int i = 0; i < N_BRANCHS; i++) {
            cleanup(&(b->children[i]));
        }
        free(b->children);
    } else {
        for (int i = 0; i < N_BRANCHS; i++) {
            if( b->cells[i] != NULL ) free( b->cells[i] );
        }
        free(b->cells);
    }
}

void add_branch(branch* b, float size){
    if( size / 2 <= 1 ) {
        b->children = NULL;
        b->cells = malloc( N_BRANCHS * sizeof(cell*) );

        for (int i = 0; i < N_BRANCHS; i++) {
            b->cells[i] = NULL;
        }
    } else {
        b->children = malloc( N_BRANCHS * sizeof(branch) );
        b->cells = NULL;
        for (int i = 0; i < N_BRANCHS; i++) {
            add_branch(&(b->children[i]), size / 2);
        }
    }
}

cell** find_path(branch* root, int size, short x, short y, short z) {
    short acmx, acmy, acmz;
    float s = (float)size;
    branch* aux = root;

    acmx = acmy = acmz = 0;

    acmx = acmy = acmz = 0;
    while ( true ) {
        s /= 2;

        if ( x >= acmx + s ) {

            if ( y >= acmy + s ) {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        return &(aux->cells[3]);
                    } else {
                        aux = &(aux->children[3]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        return &(aux->cells[7]);
                    } else {
                        aux = &(aux->children[7]);
                    }

                }

                acmy += s;

            } else {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        return &(aux->cells[1]);
                    } else {
                        aux = &(aux->children[1]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        return &(aux->cells[5]);
                    } else {
                        aux = &(aux->children[5]);
                    }

                }

            }

            acmx += s;

        } else {

            if ( y >= acmy + s ) {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        return &(aux->cells[2]);
                    } else {
                        aux = &(aux->children[2]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        return &(aux->cells[6]);
                    } else {
                        aux = &(aux->children[6]);
                    }

                }

                acmy += s;

            } else {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        return &(aux->cells[0]);
                    } else {
                        aux = &(aux->children[0]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        return &(aux->cells[4]);
                    } else {
                        aux = &(aux->children[4]);
                    }

                }

            }

        }
    }
}

cell* add_cell(branch* root, int size, short x, short y, short z) {
    cell* c = malloc( sizeof(cell) );
    float acmx, acmy, acmz;
    float s = (float)size;
    branch* aux = root;

    if (c == NULL) return NULL;

    c->alive[0] = true;
    c->alive[1] = false;
    c->checked[0] = false;
    c->checked[1] = false;

    // printf("[CELL] x: %hd\ty: %hd\tz: %hd\t\n", x, y, z);

    acmx = acmy = acmz = 0.0;
    while ( true ) {
        s /= 2;

        // printf("------ s: %f\tacmx + s: %f\tacmy + s: %f\tacmz + s: %f\n", s, (acmx + s), (acmy + s), (acmz + s));

        if ( x >= acmx + s ) {

            if ( y >= acmy + s ) {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        aux->cells[3] = c;
                        return aux->cells[3];
                    } else {
                        aux = &(aux->children[3]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        aux->cells[7] = c;
                        return aux->cells[7];
                    } else {
                        aux = &(aux->children[7]);
                    }

                }

                acmy += s;

            } else {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        aux->cells[1] = c;
                        return aux->cells[1];
                    } else {
                        aux = &(aux->children[1]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        aux->cells[5] = c;
                        return aux->cells[5];
                    } else {
                        aux = &(aux->children[5]);
                    }

                }

            }

            acmx += s;

        } else {

            if ( y >= acmy + s ) {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        aux->cells[2] = c;
                        return aux->cells[2];
                    } else {
                        aux = &(aux->children[2]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        aux->cells[6] = c;
                        return aux->cells[6];
                    } else {
                        aux = &(aux->children[6]);
                    }

                }

                acmy += s;

            } else {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        aux->cells[0] = c;
                        return aux->cells[0];
                    } else {
                        aux = &(aux->children[0]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        aux->cells[4] = c;
                        return aux->cells[4];
                    } else {
                        aux = &(aux->children[4]);
                    }

                }

            }

        }
    }

    return c;
}

branch* receive_input(char const* f) {
    FILE* file = fopen(f,"r");
    short x, y, z;
    branch* root = malloc( sizeof(branch) );
    cell* aux;
    int size;

    if ( file == NULL || root == NULL ) return NULL;

    printf("[RECEIVE INPUT]\n");

    fscanf(file, "%d", &(size));

    add_branch(root, (float)size);

    while( fscanf(file, "%hd %hd %hd", &x, &y, &z) != EOF ) {
        if( (aux = add_cell(root, size, x, y, z) ) == NULL) return NULL;
        aux->neighbors[0] = find_path(root, size, (x - 1) % size, y, z);
        aux->neighbors[2] = find_path(root, size, (x + 1) % size, y, z);
        aux->neighbors[1] = find_path(root, size, x, (y - 1) % size, z);
        aux->neighbors[3] = find_path(root, size, x, (y + 1) % size, z);
        aux->neighbors[4] = find_path(root, size, x, y, (z - 1) % size);
        aux->neighbors[5] = find_path(root, size, x, y, (z + 1) % size);
    }

    fclose(file);

    return root;
}

void cycle(int it, branch* root) {

}

void start_game(int it, branch* root) {
    printf("[START GAME]\n");

    for (int i = 0; i < it; i++) {
        cycle(it, root);
    }

    cleanup(root);
    free(root);
}

int main(int argc, char const *argv[]) {
    branch* root;
    int it;

    if (argc > 2) {
        if ( (it = atoi(argv[2])) == 0 ) return 1;
        if ( ( root = receive_input( argv[1] ) ) != NULL ) start_game(it, root);
    } else {
        printf("Not enough args: requires 2\n");
    }

    return 0;
}
