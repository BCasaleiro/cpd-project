#include <iostream>
#include <math.h>
#include <map>

using namespace std;

char* hash_function(short x, short y, short z, float size) {
    char* hash = (char*) malloc( (ceil( log2 (size) ) + 1) * sizeof(char) );
    int acmx, acmy, acmz, i;

    acmx = acmy = acmz = i = 0;
    while (size /= 2 >= 1) {
        if ( x >= acmx + size ) {
            if ( y >= acmy + size ) {
                if ( z >= acmz + size ) {
                    hash[i++] = '7';
                } else {
                    hash[i++] = '3';
                }
            } else {
                if ( z >= acmz + size ) {
                    hash[i++] = '5';
                } else {
                    hash[i++] = '1';
                }
            }
        } else {
            if ( y >= acmy + size ) {
                if ( z >= acmz + size ) {
                    hash[i++] = '6';
                } else {
                    hash[i++] = '2';
                }
            } else {
                if ( z >= acmz + size ) {
                    hash[i++] = '4';
                } else {
                    hash[i++] = '0';
                }
            }
        }
    }

    if ( i <= ceil( log2 (size) ) ) hash[i] = '\0';

    return hash;
}

int receive_input(const char* fp, map<char*,bool>* cells) {
    FILE* file = fopen(fp, "r");
    short x, y, z;
    int size;

    printf("[RECEIVE INPUT]\n");

    if ( file == NULL) return -1;

    fscanf(file, "%d", &size);

    while( fscanf(file, "%hd %hd %hd", &x, &y, &z) != EOF ) {
        printf("%s\n", hash_function(x, y, z, size));
    }

    fclose(file);

    return size;
}

void start_game(int it, int size) {
    int i;

    for (i = 0; i < it; i++) {

    }
}

int main(int argc, char const *argv[]) {
    map<char*,bool> cells;
    int it, size;

    if (argc > 2) {
        if ( (it = atoi(argv[2])) == 0 ) return 1;
        if ( (size = receive_input( argv[1], &cells ) ) != -1 ) start_game(it, size);
    } else {
        printf("Not enough args: requires 2\n");
    }

    return 0;
}
