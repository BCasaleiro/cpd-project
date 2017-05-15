#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(index,p,n) (((p)*((index)+1)-1)/(n))

int countNeighbours(Tree ****hash, localization x, int n);

void preOrder(Node *root, Tree ****hash, int i, int j, Row *insert, Row *delete, int n);

void nextGen(Tree ****hash, Row *insert, Row *delete, int n);

int readFile(Tree ****hash, char *file, int id, int nprocs);
