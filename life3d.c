#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char* receive_input(char const* f) {
    FILE* file = fopen(f,"r");
    int x, y, z, size;

    if ( file == NULL) return NULL;

    fscanf(file, "%d", &size);

    while( fscanf(file, "%d %d %d", &x, &y, &z) != EOF ) {

    }

    fclose(file);

    return NULL;
}

void start_game() {

}

int main(int argc, char const *argv[]) {
    int it;

    if (argc > 2) {
        if ( (it = atoi(argv[2])) == 0 ) return 1;
        if ( receive_input( argv[1] ) != NULL ) start_game();
    } else {
        printf("Not enough args: requires 2\n");
    }

    return 0;
}
