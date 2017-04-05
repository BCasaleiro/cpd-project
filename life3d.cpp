#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

short* hash_to_coords(string hash, int size) {
    short* coords = (short*) malloc(3 * sizeof(short));
    float x, y, z;
    size_t i;
    float s;

    s = (float) size;
    i = x = y = z = 0;
    while ( i < ceil( log2f(size) ) ) {
        s /= 2;
        if ( hash[i] == '1' || hash[i] == '3' || hash[i] == '5' || hash[i] == '7' ) {
            x += s;
        }
        if ( hash[i] == '2' || hash[i] == '3' || hash[i] == '6' || hash[i] == '7' ) {
            y += s;
        }
        if ( hash[i] == '4' || hash[i] == '5' || hash[i] == '6' || hash[i] == '7' ) {
            z += s;
        }
        i++;
    }

    coords[0] = ceil(x);
    coords[1] = ceil(y);
    coords[2] = ceil(z);

    return coords;
}

string hash_function(short x, short y, short z, float size) {
    char* hash = (char*) malloc( (ceil( log2f(size) ) + 1) * sizeof(char) );
    float acmx, acmy, acmz;
    size_t i;

    if (hash == NULL) return NULL;

    acmx = acmy = acmz = i = 0;
    while ( size >= 1 ) {
        size /= 2;
        if ( x >= acmx + size ) {
            if ( y >= acmy + size ) {
                if ( z >= acmz + size ) {
                    hash[i++] = '7';
                    acmz += size;
                } else {
                    hash[i++] = '3';
                }
                acmy += size;
            } else {
                if ( z >= acmz + size ) {
                    hash[i++] = '5';
                    acmz += size;
                } else {
                    hash[i++] = '1';
                }
            }
            acmx += size;
        } else {
            if ( y >= acmy + size ) {
                if ( z >= acmz + size ) {
                    hash[i++] = '6';
                    acmz += size;
                } else {
                    hash[i++] = '2';
                }
                acmy += size;
            } else {
                if ( z >= acmz + size ) {
                    hash[i++] = '4';
                    acmz += size;
                } else {
                    hash[i++] = '0';
                }
            }
        }
    }

    hash[i] = '\0';

    return hash;
}

int receive_input(const char* fp, map<string,uint8_t>* cells) {
    FILE* file = fopen(fp, "r");
    short x, y, z;
    int size;

    if ( file == NULL) return -1;

    fscanf(file, "%d", &size);

    while( fscanf(file, "%hd %hd %hd", &x, &y, &z) != EOF ) {
        (*cells).insert( pair<string, uint8_t>(hash_function(x, y, z, (float)size), 0) );
    }

    fclose(file);

    return size;
}

void process_neighbors(map<string,uint8_t>* cells, vector<string>* zombies, string hash, int size) {
    float x, y, z;
    string aux;
    size_t i;
    float s;

    s = (float) size;
    i = x = y = z = 0;
    while ( i < ceil( log2f(size) ) ) {
        s /= 2;
        if ( hash[i] == '1' || hash[i] == '3' || hash[i] == '5' || hash[i] == '7' ) {
            x += s;
        }
        if ( hash[i] == '2' || hash[i] == '3' || hash[i] == '6' || hash[i] == '7' ) {
            y += s;
        }
        if ( hash[i] == '4' || hash[i] == '5' || hash[i] == '6' || hash[i] == '7' ) {
            z += s;
        }
        i++;
    }

    x = ceil(x);
    y = ceil(y);
    z = ceil(z);

    if ( x - 1 < 0) {
        aux = hash_function(size - 1, y, z, (float)size);
        if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
        else (*zombies).push_back(aux);
    } else {
        aux = hash_function(x - 1, y, z, (float)size);
        if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
        else (*zombies).push_back(aux);
    }

    aux = hash_function(x + 1 % size, y, z, (float)size);
    if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
    else (*zombies).push_back(aux);

    if ( y - 1 < 0) {
        aux = hash_function(x, size - 1, z, (float)size);
        if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
        else (*zombies).push_back(aux);
    } else {
        aux = hash_function(x, y - 1, z, (float)size);
        if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
        else (*zombies).push_back(aux);
    }

    aux = hash_function(x, y + 1 % size, z, (float)size);
    if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
    else (*zombies).push_back(aux);

    if ( z - 1 < 0) {
        aux = hash_function(x, y, size - 1, (float)size);
        if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
        else (*zombies).push_back(aux);
    } else {
        aux = hash_function(x, y, z - 1, (float)size);
        if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
        else (*zombies).push_back(aux);
    }

    aux = hash_function(x, y, z + 1 % size, (float)size);
    if ( (*cells).count(aux) > 0 ) (*cells)[hash] += 1;
    else (*zombies).push_back(aux);
}

void bring_from_the_dead(map<string,uint8_t>* cells, vector<string>* zombies) {
    string aux = (*zombies)[0];
    int counter = 0;

    for (vector<string>::iterator it = (*zombies).begin() + 1 ; it != (*zombies).end(); ++it) {
        if ( *it == aux ) {
            counter++;
        } else {
            if (counter == 2 || counter == 3) {
                (*cells)[*it] = 0;
            } else {
                counter = 0;
                aux = *it;
            }
        }
    }
}

bool cmp (short* a, short* b) {
    if ( a[0] <  b[0] ) {
        return true;
    } else if ( a[0] == b[0] ) {
        if ( a[1] < b[1] ) {
            return true;
        } else if ( a[1] == b[1] ) {
            if ( a[2] < b[2] ) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void print_live_cells(map<string,uint8_t>* cells, int size) {
    vector<short*> vec;

    for (map<string,uint8_t>::iterator it=(*cells).begin(); it!=(*cells).end(); ++it) {
        vec.push_back( hash_to_coords(it->first, size) );
    }

    sort(vec.begin(), vec.end(), cmp);

    for (vector<short*>::iterator it=vec.begin(); it!=vec.end(); ++it) {
        printf("%d %d %d\n", (*it)[0], (*it)[1], (*it)[2]);
    }
}

void start_game(map<string,uint8_t>* cells, int n, int size) {
    vector<string> zombies;
    int i;
    short* coords;

    for (i = 0; i < n; i++) {
        for (map<string,uint8_t>::iterator it=(*cells).begin(); it!=(*cells).end(); ++it) {
            process_neighbors(cells, &zombies, it->first, size);
            coords = hash_to_coords(it->first, size);
        }

        if (zombies.size() > 0) {
            sort(zombies.begin(), zombies.end());
            bring_from_the_dead(cells, &zombies);
            zombies.erase (zombies.begin(),zombies.end());
        }

        for (map<string,uint8_t>::iterator it=(*cells).begin(); it!=(*cells).end(); ++it) {
            if (it->second < 2 || it->second > 4) {
                (*cells).erase(it);
            } else {
                it->second = 0;
            }
        }
    }

    print_live_cells(cells, size);
}

int main(int argc, char const *argv[]) {
    map<string,uint8_t> cells;
    int it, size;

    if (argc > 2) {
        if ( (it = atoi(argv[2])) == 0 ) return 1;
        if ( (size = receive_input( argv[1], &cells ) ) != -1 ) start_game(&cells, it, size);
    } else {
        printf("Not enough args: requires 2\n");
    }

    return 0;
}
