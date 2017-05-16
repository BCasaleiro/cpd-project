typedef struct _Node {
  struct _Node *left;
  struct _Node *right;
  int height;
  int data;
} Node;

typedef struct _Tree {
  struct _Node *root;
  int size;
} Tree;

int height( Node* n );

void initTree(Tree *tree);

int max( int l, int r);

Node* newNode(int key);

Node *rightRotate(Node *y);

Node *leftRotate(Node *x);

int getBalance(Node *N);

Node* insertTree(int key, Node* node, int * size);

void display_avl(Node* t, int x, int y, int id, int nprocs);

void printTree(Tree ****hash, int n, int id, int nprocs);

void fdisplay_avl(Node* t, int x, int y, FILE *fp);

void fprintTree(Tree ****hash, int n, FILE *fp);

Node *find(int e, Node* t );

void dispose(Node* t);

void freeTree(Tree ****hash, int n);

Node *minValueNode(Node *node);

Node* deleteNode(Node* root, int key);
