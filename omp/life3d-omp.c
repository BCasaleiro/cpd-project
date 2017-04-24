#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include"list.h"
#include"tree.h"
#include"aux.h"

omp_lock_t lck_a;

int main(int argc, char *argv[]) {

  float start = omp_get_wtime();
  int th=omp_get_max_threads();
  omp_init_lock(&lck_a);

  if (argc != 3) {
    printf("Usage: life3d-omp <infile> <iterations>.\n");
    exit(-1);
  }

  char name[246];
  strcpy(name, argv[1]);
  int k = strtol(argv[2],NULL,10);

  Tree ****hash = (Tree****)malloc(sizeof(Tree****));
  //Tree ****hash1 = (Tree****)malloc(sizeof(Tree****));
  int i;
  int n;
  FILE *fp;
  fp = fopen("result.out", "w");
  Row **insert=(Row**)malloc(th*sizeof(Row*));
  for (i = 0; i < th; i++) {
    insert[i]=(Row*)malloc(sizeof(Row));
  }
  Row **delete=(Row**)malloc(th*sizeof(Row*));
  for (i = 0; i < th; i++) {
    delete[i]=(Row*)malloc(sizeof(Row));
  }
  n = readFile(hash, name);
  for (i = 0; i < k; i++){
    if (i==0) {
    }else{
      //hash=hash1;
      //hash1 = (Tree****)malloc(sizeof(Tree****));
    }
    //printTree(hash, n);
    //printf("ola\n");
    nextGen(hash, insert, delete, n);
    //printf("ola\n");
    //freeTree(hash, n);
  }
  fprintTree(hash,n, fp);
  for (i = 0; i < th; i++) {
    free(insert[i]);
    free(delete[i]);
  }
  free(insert);
  free(delete);
  freeTree(hash, n);
  fclose(fp);
  omp_destroy_lock(&lck_a);
  float end = omp_get_wtime();
  float diff = end - start;
  printf("%f\n", diff);
  return 0;
}
