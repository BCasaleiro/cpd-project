#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"
#include"list.h"

/** Count the alive neighboors of a cell */
int countNeighbours(Tree ****hash, localization x, int n) {
    int i = 0;

    if(x.x==0){
        if(find(x.z, (*hash)[n-1][x.y]->root)!=NULL){
            i++;
        }
    }else{
        if(find(x.z, (*hash)[x.x-1][x.y]->root)!=NULL){
            i++;
        }
    }

    if (x.x==n-1) {
        if(find(x.z, (*hash)[0][x.y]->root)!=NULL){
            i++;
        }
    }else{
        if(find(x.z, (*hash)[x.x+1][x.y]->root)!=NULL){
            i++;
        }
    }

    if (x.y==0) {
        if(find(x.z, (*hash)[x.x][n-1]->root)!=NULL){
            i++;
        }
    }else{
        if(find(x.z, (*hash)[x.x][x.y-1]->root)!=NULL){
            i++;
        }
    }

    if (x.y==n-1) {
        if(find(x.z, (*hash)[x.x][0]->root)!=NULL){
            i++;
        }
    }else{
        if(find(x.z, (*hash)[x.x][x.y+1]->root)!=NULL){
            i++;
        }
    }

    if (x.z==0) {
        if(find(n-1, (*hash)[x.x][x.y]->root)!=NULL){
            i++;
        }
    }else{
        if(find(x.z-1, (*hash)[x.x][x.y]->root)!=NULL){
            i++;
        }
    }

    if (x.z==n-1) {
        if(find(0, (*hash)[x.x][x.y]->root)!=NULL){
            i++;
        }
    }else{
        if(find(x.z+1, (*hash)[x.x][x.y]->root)!=NULL){
            i++;
        }
    }

    return i;
}

/** Check */
void preOrder(Node *root, Tree ****hash, int i, int j, Row *insert, int n) {
    localization x;
    int c;
    if(root != NULL) {
        x.x = i;
        x.y = j;
        x.z = root->data;
        c = countNeighbours(hash,x, n);
        if(c == 4) {
            add(insert, x);
        }

        if (i == n - 1) {
            x.x = 0;
            x.y = j;
            x.z = root->data;
        }else{
            x.x = i + 1;
            x.y = j;
            x.z = root->data;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (i==0) {
            x.x = n-1;
            x.y = j;
            x.z = root->data;
        }else{
            x.x = i - 1;
            x.y = j;
            x.z = root->data;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (j==n-1) {
            x.x=i;x.y=0;x.z=root->data;
        }else{
            x.x=i;x.y=j+1;x.z=root->data;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (j==0) {
            x.x=i;x.y=n-1;x.z=root->data;
        }else{
            x.x=i;x.y=j-1;x.z=root->data;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (root->data==n-1) {
            x.x=i;x.y=j;x.z=0;
        }else{
            x.x=i;x.y=j;x.z=root->data+1;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (root->data==0) {
            x.x=i;x.y=j;x.z=n-1;
        }else{
            x.x=i;x.y=j;x.z=root->data-1;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        preOrder(root->left, hash, i, j, insert, n);
        preOrder(root->right, hash, i, j, insert, n);
    }
}

/** Compute Next Generation */
void nextGen(Tree ****hash, Row *insert, int n, Tree ****hash1){
    int i,j;
    List *aux;

    /** For each cell in a 2d dimensional space aloc a pointer of pointer of pointer */
    (*hash1) = (Tree***)malloc(n*n*sizeof(Tree**));
    for (i = 0; i < n; i++) {
        /** For each row aloc a pointer of pointer */
        (*hash1)[i]=(Tree**)malloc(n*sizeof(Tree*));
        for (j = 0; j < n; j++) {
            /** For each column in that row aloc a Tree pointer and initialize it */
            (*hash1)[i][j]=(Tree*)malloc(sizeof(Tree));
            initTree((*hash1)[i][j]);
        }
    }

    /** For each coord (x, y) add to list the curret and the potentialy dead cells
        that may become alive */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            preOrder((*hash)[i][j]->root, hash, i, j, insert, n);
        }
    }

    /** For each node on the list add or ignore if already exists a new live cell */
    for (aux = insert->first; aux!=NULL; aux=aux->next) {
        (*hash1)[aux->data.x][aux->data.y]->root = insertTree(aux->data.z, (*hash1)[aux->data.x][aux->data.y]->root);
        (*hash1)[aux->data.x][aux->data.y]->size=((*hash1)[aux->data.x][aux->data.y]->size) + 1;
    }
}

/** Read File */
int readFile(Tree ****hash, char *file) {
    FILE *fp;
    int x, y, z, n;
    int i, j;

    /** Open testcase file */
    fp=fopen(file,"r");

    /** Get world size */
    fscanf(fp, "%d", &n);

    /** For each cell in a 2d dimensional space aloc a pointer of pointer of pointer */
    (*hash) = (Tree***)malloc(n*n*sizeof(Tree**));
    for (i = 0; i < n; i++) {
        /** For each row aloc a pointer of pointer */
        (*hash)[i]=(Tree**)malloc(n*sizeof(Tree*));
        for (j = 0; j < n; j++) {
            /** For each column in that row aloc a Tree pointer and initialize it */
            (*hash)[i][j]=(Tree*)malloc(sizeof(Tree));
            initTree((*hash)[i][j]);
        }
    }

    /** Initialize the given cells */
    while (fscanf(fp,"%d %d %d", &x, &y, &z)==3) {
        (*hash)[x][y]->root=insertTree(z,(*hash)[x][y]->root);
        (*hash)[x][y]->size=((*hash)[x][y]->size) + 1;
    }

    /** Close testcase file */
    fclose(fp);

    return n;
}
