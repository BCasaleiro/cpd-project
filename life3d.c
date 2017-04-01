#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define N_BRANCHS 8
#define N_NEIGHBORS 6

typedef struct cell cell;
typedef struct cell {
    cell** neighbors[6];
    bool alive;
    short x, y, z;
    uint8_t neig_counter;
} cell;

typedef struct branch branch;
typedef struct branch {
    branch* children;
    cell** cells;
} branch;

typedef struct potential potential;
typedef struct potential {
    short x, y, z;
    cell** c;
} potential;

int megasize;

void cleanup(branch* b) {
    int i;
    if ( b->children != NULL ) {
        for (i = 0; i < N_BRANCHS; i++) {
            cleanup(&(b->children[i]));
        }
        free(b->children);
    } else {
        for (i = 0; i < N_BRANCHS; i++) {
            if( b->cells[i] != NULL ) free( b->cells[i] );
        }
        free(b->cells);
    }
}

void add_branch(branch* b, float size){
    int i;
    if( size / 2 <= 1 ) {
        b->children = NULL;
        b->cells = malloc( N_BRANCHS * sizeof(cell*) );

        for (i = 0; i < N_BRANCHS; i++) {
            b->cells[i] = NULL;
        }
    } else {
        b->children = malloc( N_BRANCHS * sizeof(branch) );
        b->cells = NULL;
        for (i = 0; i < N_BRANCHS; i++) {
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

cell* add_cell(branch* root, int size, short x, short y, short z, bool alive) {
    cell* c = malloc( sizeof(cell) );
    float acmx, acmy, acmz;
    float s = (float)size;
    branch* aux = root;

    if (c == NULL) return NULL;

    c->alive = alive;
    c->x = x;
    c->y = y;
    c->z = z;
    c->neig_counter = 0;

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

void add_neighbors(cell* c, branch* root, int size, short x, short y, short z) {
    if ( x - 1 < 0 ) {
        c->neighbors[0] = find_path(root, size, size - 1, y, z);
    } else {
        c->neighbors[0] = find_path(root, size, (x - 1) % size, y, z);
    }
    c->neighbors[2] = find_path(root, size, (x + 1) % size, y, z);
    if ( y - 1 < 0 ) {
        c->neighbors[1] = find_path(root, size, x, size - 1, z);
    } else {
        c->neighbors[1] = find_path(root, size, x, (y - 1) % size, z);
    }
    c->neighbors[3] = find_path(root, size, x, (y + 1) % size, z);
    if ( z - 1 < 0 ) {
        c->neighbors[4] = find_path(root, size, x, y, size - 1);
    } else {
        c->neighbors[4] = find_path(root, size, x, y, (z - 1) % size);
    }
    c->neighbors[5] = find_path(root, size, x, y, (z + 1) % size);
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
    megasize = size;

    add_branch(root, (float)size);

    while( fscanf(file, "%hd %hd %hd", &x, &y, &z) != EOF ) {
        if ( (aux = add_cell(root, size, x, y, z, true) ) == NULL) return NULL;
        add_neighbors(aux, root, size, x, y, z);
    }

    fclose(file);

    return root;
}

cell* count_neighbors(branch* root, int size, short x, short y, short z, bool alive) {
    cell* c = malloc( sizeof(cell) );
    float acmx, acmy, acmz;
    float s = (float)size;
    branch* aux = root;

    if (c == NULL) return NULL;

    c->alive = alive;
    c->x = x;
    c->y = y;
    c->z = z;
    c->neig_counter = 0;

    acmx = acmy = acmz = 0.0;
    while ( true ) {
        s /= 2;

        if ( x >= acmx + s ) {

            if ( y >= acmy + s ) {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        if ( aux->cells[3] == NULL ) {
                            aux->cells[3] = c;
                            return aux->cells[3];
                        } else {
                            aux->cells[3]->neig_counter += 1;
                        }

                        return aux->cells[3];
                    } else {
                        aux = &(aux->children[3]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        if ( aux->cells[7] == NULL ) {
                            aux->cells[7] = c;
                            return aux->cells[7];
                        } else {
                            aux->cells[7]->neig_counter += 1;
                        }
                    } else {
                        aux = &(aux->children[7]);
                    }

                }

                acmy += s;

            } else {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        if ( aux->cells[1] == NULL ) {
                            aux->cells[1] = c;
                            return aux->cells[1];
                        } else {
                            aux->cells[1]->neig_counter += 1;
                        }
                    } else {
                        aux = &(aux->children[1]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        if ( aux->cells[5] == NULL ) {
                            aux->cells[5] = c;
                            return aux->cells[5];
                        } else {
                            aux->cells[5]->neig_counter += 1;
                        }
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
                        if ( aux->cells[2] == NULL ) {
                            aux->cells[2] = c;
                            return aux->cells[2];
                        } else {
                            aux->cells[2]->neig_counter += 1;
                        }
                    } else {
                        aux = &(aux->children[2]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        if ( aux->cells[6] == NULL ) {
                            aux->cells[6] = c;
                            return aux->cells[6];
                        } else {
                            aux->cells[6]->neig_counter += 1;
                        }
                    } else {
                        aux = &(aux->children[6]);
                    }

                }

                acmy += s;

            } else {

                if ( z >= acmz + s ) {

                    if ( s <= 1 ) {
                        if ( aux->cells[0] == NULL ) {
                            aux->cells[0] = c;
                            return aux->cells[0];
                        } else {
                            aux->cells[0]->neig_counter += 1;
                        }
                    } else {
                        aux = &(aux->children[0]);
                    }

                    acmz += s;
                } else {

                    if ( s <= 1 ) {
                        if ( aux->cells[4] == NULL ) {
                            aux->cells[4] = c;
                            return aux->cells[4];
                        } else {
                            aux->cells[4]->neig_counter += 1;
                        }
                    } else {
                        aux = &(aux->children[4]);
                    }

                }

            }

        }
    }

    return c;
}

void process_cell(branch* b, cell* c) {
    int i;
    for (i = 0; i < N_NEIGHBORS; i++) {
        if ( c->neighbors[i] != NULL && *c->neighbors[i] != NULL ) {
            c->neig_counter += 1;
        } else {
            switch (i) {
                case 0:
                    if ( c->x - 1 < 0 ) {
                        count_neighbors(b, megasize, megasize - 1, c->y, c->z, false);
                    } else {
                        count_neighbors(b, megasize, (c->x - 1) % megasize, c->y, c->z, false);
                    }
                    break;
                case 1:
                    if ( c->y - 1 < 0 ) {
                        count_neighbors(b, megasize, c->x, megasize - 1, c->z, false);
                    } else {
                        count_neighbors(b, megasize, c->x, (c->y - 1) % megasize, c->z, false);
                    }
                    break;
                case 2:
                    count_neighbors(b, megasize, (c->x - 1) % megasize, c->y, c->z, false);
                    break;
                case 3:
                    count_neighbors(b, megasize, c->x, (c->y - 1) % megasize, c->z, false);
                    break;
                case 4:
                    if ( c->z - 1 < 0 ) {
                        count_neighbors(b, megasize, c->x, c->y, megasize - 1, false);
                    } else {
                        count_neighbors(b, megasize, c->x, c->y, (c->z - 1) % megasize, false);
                    }
                    break;
                case 5:
                    count_neighbors(b, megasize, c->x, c->y, (c->z - 1) % megasize, false);
                    break;
                default:
                    printf("ERRO!!\n");
            }

        }
    }
}

void clean_cycle(branch* b) {
    int i;
    if ( b->children != NULL ) {
        for (i = 0; i < N_BRANCHS; i++) {
            clean_cycle(&(b->children[i]));
        }
    } else {
        for (i = 0; i < N_BRANCHS; i++) {
            if ( b->cells[i] != NULL ) {
                // 8 é porque já foi processado neste ciclo
                if (b->cells[i]->alive && b->cells[i]->neig_counter >= 2 && b->cells[i]->neig_counter <= 4) {
                    b->cells[i]->neig_counter = 0;
                } else if (!(b->cells[i]->alive) && b->cells[i]->neig_counter == 2 && b->cells[i]->neig_counter == 3) {
                    b->cells[i]->alive = true;
                    b->cells[i]->neig_counter = 0;
                } else {
                    free(b->cells[i]);
                }
            }
        }
    }
}

void cycle(branch* b, int size) {
    int i;
    if ( b->children != NULL ) {
        for (i = 0; i < N_BRANCHS; i++) {
            cycle(&(b->children[i]), size);
        }
    } else {
        for (i = 0; i < N_BRANCHS; i++) {
            if (b->cells[i] != NULL) {
                process_cell(b, b->cells[i]);
            }
        }
    }
}

void start_game(branch* root, int it, int size) {
    int i;
    printf("[START GAME]\n");

    for (i = 0; i < it; i++) {
        cycle(root, size);
        printf("cycle\n");
        clean_cycle(root);
    }

    cleanup(root);
    free(root);
}

int main(int argc, char const *argv[]) {
    branch* root;
    int it;

    if (argc > 2) {
        if ( (it = atoi(argv[2])) == 0 ) return 1;
        if ( ( root = receive_input( argv[1] ) ) != NULL ) start_game(root, it, megasize);
    } else {
        printf("Not enough args: requires 2\n");
    }

    return 0;
}
