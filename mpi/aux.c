#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"
#include"list.h"
#include"aux.h"

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


void preOrderf1(Node *root, Tree ****hash, int i, int j, Row* insert, Row* delete, int n) {
    localization x;
    int c;
    if(root != NULL) {
        x.x = i;
        x.y = j;
        x.z = root->data;

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
        preOrderf1(root->left, hash, i, j, insert, delete, n);
        preOrderf1(root->right, hash, i, j, insert, delete, n);
    }
}

void preOrderi1(Node *root, Tree ****hash, int i, int j, Row* insert, Row* delete, int n) {
    localization x;
    int c;
    if(root != NULL) {
        x.x = i;
        x.y = j;
        x.z = root->data;

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
        preOrderi1(root->left, hash, i, j, insert, delete, n);
        preOrderi1(root->right, hash, i, j, insert, delete, n);
    }
}

void preOrderi(Node *root, Tree ****hash, int i, int j, Row* insert, Row* delete, int n) {
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

        /*if (i==n-1) {
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
        }*/

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

        preOrderi(root->left, hash, i, j, insert, delete, n);
        preOrderi(root->right, hash, i, j, insert, delete, n);
    }
}

void preOrderf(Node *root, Tree ****hash, int i, int j, Row* insert, Row* delete, int n) {
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

        /*if (i==0) {
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
        }*/

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

        preOrderf(root->left, hash, i, j, insert, delete, n);
        preOrderf(root->right, hash, i, j, insert, delete, n);
    }
}

void preOrder(Node *root, Tree ****hash, int i, int j, Row* insert, Row* delete, int n) {
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

void nextGen(Tree ****hash, Row *insert, Row *delete, int n, int id, int nprocs) {
    int i, j;
    List *aux;

    /** Initialize its insert and delete Linked-Lists */
    init(insert);
    init(delete);

    /** For each coord (x, y) add to list the curret and the potentialy dead cells
        that may become alive */
    for (i = 1; i < BLOCK_SIZE(id, nprocs, n) - 1; i++) {
        for (j = 0; j < n; j++) {
            preOrder((*hash)[i][j]->root, hash, i, j, insert, delete, n);
        }
    }

    for (j = 0; j < n; j++) {
        preOrderi((*hash)[0][j]->root, hash, 0, j, insert, delete, n);
    }

    for (j = 0; j < n; j++) {
        preOrderf((*hash)[BLOCK_SIZE(id, nprocs, n)][j]->root, hash, BLOCK_SIZE(id, nprocs, n), j, insert, delete, n);
    }

    /*if(pcGetID()==0){
      for (j = 0; j < n; j++) {
          preOrderi1((*hash)[n][j]->root, hash, n, j, insert, delete, n);
      }
    }else{
      for (j = 0; j < n; j++) {
          preOrderi1((*hash)[(id*n)/np-1][j]->root, hash, (id*n)/np-1, j, insert, delete, n);
      }
    }

    if(pcGetID()==np-1){
      for (j = 0; j < n; j++) {
          preOrderf1((*hash)[1][j]->root, hash, 1, j, insert[tid], delete[tid], n);
      }
    }else{
      for (j = 0; j < n; j++) {
          preOrderf1((*hash)[(id*n)/np+1][j]->root, hash, (id*n)/np+1, j, insert[tid], delete[tid], n);
      }
    }*/
    /** For each node on the delete list remove it */
    for (aux = delete->first; aux!=NULL; aux=aux->next) {
        (*hash)[aux->data.x][aux->data.y]->root = deleteNode((*hash)[aux->data.x][aux->data.y]->root,aux->data.z);
        (*hash)[aux->data.x][aux->data.y]->size=((*hash)[aux->data.x][aux->data.y]->size) - 1;
    }

    /** For each node on the insert list add or ignore if already exists a new live cell */
 
   for (aux = insert->first; aux!=NULL; aux=aux->next) {
        (*hash)[aux->data.x][aux->data.y]->root = insertTree(aux->data.z, (*hash)[aux->data.x][aux->data.y]->root);
        (*hash)[aux->data.x][aux->data.y]->size=((*hash)[aux->data.x][aux->data.y]->size) + 1;
    }

    /** Free threads insert and delete Linked-Lists */
    freeList(insert);
    freeList(delete);


}

/** Read File */
int readFile(Tree ****hash, char *file, int id, int nprocs) {
    FILE *fp;
    int x, y, z, n;
    int i, j;

    /** Open testcase file */
    fp=fopen(file,"r");

    /** Get world size */
    fscanf(fp, "%d", &n);

    /** For each cell in a 2d dimensional space aloc a pointer of pointer of pointer */
    (*hash) = (Tree***)malloc( ( 2 + BLOCK_SIZE(id, nprocs, n)) * n *sizeof(Tree**));
    for (i = 0; i < BLOCK_SIZE(id, nprocs, n) + 2; i++) {
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
	//printf("[%d] %d %d\n", id, BLOCK_OWNER( x, nprocs, n), x);
        if( BLOCK_OWNER( x, nprocs, n) == id ) {
          (*hash)[1 + x - BLOCK_LOW(id, nprocs, n)][y]->root=insertTree(z,(*hash)[1 + x - BLOCK_LOW(id, nprocs, n)][y]->root);
          (*hash)[1 + x - BLOCK_LOW(id, nprocs, n)][y]->size=((*hash)[1 + x - BLOCK_LOW(id, nprocs, n)][y]->size) + 1;
        }
    }

    /** Close testcase file */
    fclose(fp);

    return n;
}
