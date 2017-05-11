#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include"tree.h"
#include"list.h"

extern omp_lock_t lck_a;

/** Count the alive neighboors of a cell */
int countNeighbours(Tree ****hash, localization x, int n) {
    int i=0;
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

/** Insert nodes to kill in the delete Linked-List
    and to revive in the insert Linked-List */
void preOrder(Node *root, Tree ****hash, int i, int j, Row *insert, Row *delete, int n) {
    localization x;
    int c;
    if(root != NULL) {
        x.x = i;
        x.y = j;
        x.z = root->data;
        c = countNeighbours(hash,x, n);
        if(c<2||c>4){
            add(delete, x);
        }

        if (i==n-1) {
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
            x.x = i;
            x.y = 0;
            x.z = root->data;
        }else{
            x.x = i;
            x.y = j+1;
            x.z = root->data;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (j==0) {
            x.x = i;
            x.y = n-1;
            x.z = root->data;
        }else{
            x.x = i;
            x.y = j-1;
            x.z = root->data;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (root->data==n-1) {
            x.x = i;
            x.y = j;
            x.z = 0;
        }else{
            x.x = i;
            x.y = j;
            x.z = root->data+1;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        if (root->data==0) {
            x.x = i;
            x.y = j;
            x.z = n-1;
        }else{
            x.x = i;
            x.y = j;
            x.z = root->data-1;
        }
        c = countNeighbours(hash,x, n);
        if (c==2||c==3) {
            add(insert, x);
        }

        preOrder(root->left, hash, i, j, insert, delete, n);
        preOrder(root->right, hash, i, j, insert, delete, n);
    }
}

void nextGen(Tree ****hash, Row **insert, Row **delete, int n) {
    int i, j, tid;
    List *aux;

    #pragma omp parallel private(tid,aux)
    {
        /** Get thread number */
        tid = omp_get_thread_num();

        /** Initialize its insert and delete Linked-Lists */
        init(insert[tid]);
        init(delete[tid]);

        /** For each coord (x, y) add to list the curret and the potentialy dead cells
            that may become alive */
        #pragma omp for private(j)// schedule(dynamic, 10)
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                preOrder((*hash)[i][j]->root, hash, i, j, insert[tid], delete[tid], n);
            }
        }

        /** For each node on the delete list remove it */
        for (aux = delete[tid]->first; aux!=NULL; aux=aux->next) {
            /** Lock lock a */
            omp_set_lock(&lck_a);
            (*hash)[aux->data.x][aux->data.y]->root = deleteNode((*hash)[aux->data.x][aux->data.y]->root,aux->data.z);
            (*hash)[aux->data.x][aux->data.y]->size=((*hash)[aux->data.x][aux->data.y]->size) - 1;
            /** Unlock lock a */
            omp_unset_lock(&lck_a);
        }

        //#pragma omp barrier
        /** For each node on the insert list add or ignore if already exists a new live cell */
        for (aux = insert[tid]->first; aux!=NULL; aux=aux->next) {
            /** Lock lock a */
            omp_set_lock(&lck_a);
            (*hash)[aux->data.x][aux->data.y]->root = insertTree(aux->data.z, (*hash)[aux->data.x][aux->data.y]->root);
            (*hash)[aux->data.x][aux->data.y]->size=((*hash)[aux->data.x][aux->data.y]->size) + 1;
            /** Unlock lock a */
            omp_unset_lock(&lck_a);
        }

        /** Free threads insert and delete Linked-Lists */
        freeList(insert[tid]);
        freeList(delete[tid]);
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
