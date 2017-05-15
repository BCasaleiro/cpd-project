#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include"tree.h"

extern omp_lock_t lck_a;

/** Return the height of a node */
int height( Node* n ) {
    if( n == NULL )
    return 0;
    else
    return n->height;
}

/** Initialize tree */
void initTree(Tree *tree) {
    tree->root=NULL;
    tree->size=0;
}

/** Max between two integers */
int max( int l, int r) {
    return (l > r) ? l: r;
}

/** Create a new node */
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}

/** A utility function to right rotate subtree rooted with y */
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Return new root
    return x;
}

/** A utility function to left rotate subtree rooted with x
See the diagram given above. */
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return new root
    return y;
}

/** Get Balance factor of node N */
int getBalance(Node *N) {
    if (N == NULL)
    return 0;
    return height(N->left) - height(N->right);
}

Node* insertTree(int key, Node* node) {
    /* 1.  Perform the normal BST rotation */
    if (node == NULL){
        //omp_set_lock(&lck_a);
        return(newNode(key));
        //omp_unset_lock(&lck_a);
    }

    if (key < node->data)
    node->left  = insertTree(key,node->left);
    else if (key > node->data)
    node->right = insertTree(key,node->right);
    else // Equal keys not allowed
    return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
    height(node->right));

    /* 3. Get the balance factor of this ancestor
    node to check whether this node became
    unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->data){
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && key > node->right->data)
    return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->data)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->data)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

void display_avl(Node* t, int x, int y) {
    if (t == NULL)
    return;
    display_avl(t->left, x, y);
    printf("%d %d %d\n", x, y, t->data);

    /*if(t->left != NULL)
    printf("(L:%d)\n",t->left->data);
    if(t->right != NULL)
    printf("(R:%d)\n",t->right->data);
    printf("\n");*/


    display_avl(t->right, x, y);
}

void printTree(Tree ****hash, int n) {
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if ((*hash)[i][j]->size!=0) {
                //printf("%d %d ", i, j);
                display_avl((*hash)[i][j]->root, i, j);
                //printf("\n");
            }
        }
    }
}

void fdisplay_avl(Node* t, int x, int y, FILE *fp) {
    if (t == NULL)
    return;
    fdisplay_avl(t->left, x, y, fp);
    fprintf(fp, "%d %d %d\n", x, y, t->data);

    //if(t->left != NULL)
    //printf("(L:%d)",t->left->data);
    //if(t->right != NULL)
    //printf("(R:%d)",t->right->data);
    //printf("\n");


    fdisplay_avl(t->right, x, y, fp);
}

void fprintTree(Tree ****hash, int n, FILE *fp) {
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if ((*hash)[i][j]->size!=0) {
                //printf("%d %d ", i, j);
                fdisplay_avl((*hash)[i][j]->root, i, j, fp);
                //printf("\n");
            }
        }
    }
}

/** Find the node with coordinate z = e in the Tree t */
Node *find(int e, Node* t ) {
    if( t == NULL )
    return NULL;
    if( e < t->data )
    return find( e, t->left );
    else if( e > t->data )
    return find( e, t->right );
    else
    return t;
}

void dispose(Node* t) {
    if( t != NULL )
    {
        dispose( t->left );
        dispose( t->right );
        free( t );
    }
}

/** Free Tree */
void freeTree(Tree ****hash, int n) {
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            dispose((*hash)[i][j]->root);
            free((*hash)[i][j]);
        }
        free((*hash)[i]);
    }
    free(*hash);
    free(hash);
}

Node *minValueNode(Node *node) {
    Node *current = node;

    while (current->left != NULL)
    current = current->left;

    return current;
}

Node* deleteNode(Node* root, int key) {
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
    return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->data )
    root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->data )
    root->right = deleteNode(root->right, key);

    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            Node *temp = root->left ? root->left :
            root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
            *root = *temp; // Copy the contents of
            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            Node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->data = temp->data;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->data);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
    return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
    height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
    return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
    return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
