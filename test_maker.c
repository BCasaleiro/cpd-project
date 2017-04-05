#include <stdio.h>

#define SIZE 10000
#define LIMIT 2777

int main(int argc, char const *argv[]) {
    FILE* fw = fopen("test.txt", "w");
    FILE* fr = fopen("badjoras.in", "r");
    int x, y, z;

    fprintf(fw, "%d\n", SIZE);

    // x = y = z = -1;
    while( fscanf(fr, "%d %d %d", &x, &y, &z) != EOF ) {
        fprintf(fw, "%d %d %d\n", x % LIMIT, y % LIMIT, z % LIMIT);
    }

    fclose(fw);
    fclose(fr);

    return 0;
}
