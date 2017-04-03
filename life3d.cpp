#include <iostream>

int receive_input(const char* fp) {
    FILE* file = fopen(fp, "r");
    short x, y, z;
    int size;

    printf("[RECEIVE INPUT]\n");

    if ( file == NULL) return -1;

    fscanf(file, "%d", &size);

    while( fscanf(file, "%hd %hd %hd", &x, &y, &z) != EOF ) {

    }

    fclose(file);

    return size;
}

void start_game(int it, int size) {

}

int main(int argc, char const *argv[]) {
    int it, size;

    if (argc > 2) {
        if ( (it = atoi(argv[2])) == 0 ) return 1;
        if ( (size = receive_input( argv[1] ) ) != -1 ) start_game(it, size);
    } else {
        printf("Not enough args: requires 2\n");
    }

    return 0;
}
