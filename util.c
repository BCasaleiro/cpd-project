

Node *single_rotate_with_left(Node *k2)
{
    Node *k1 = NULL;

    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    return k1;
}



Node *single_rotate_with_right(Node *k1)
{
    Node *k2;

    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;

    k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
    k2->height = max( height( k2->right ), k1->height ) + 1;

    return k2;
}



static Node* double_rotate_with_left( Node* k3 )
{
    k3->left = single_rotate_with_right( k3->left );

    return single_rotate_with_left( k3 );
}




static Node* double_rotate_with_right( Node* k1 )
{
    k1->right = single_rotate_with_left( k1->right );

    return single_rotate_with_right( k1 );
}






Node *insertTree(int e, Node *t)
{
    if( t == NULL )
    {
        t = (Node*)malloc(sizeof(Node));
        if( t == NULL )
        {
            fprintf (stderr, "Out of memory!!! (insert)\n");
            exit(1);
        }
        else
        {
            t->data = e;
            t->height = 0;
            t->left = t->right = NULL;
        }
    }
    else if( e < t->data )
    {
        t->left = insertTree( e, t->left );
        if( height( t->left ) - height( t->right ) == 2 ){
            if( e < t->left->data )
                t = single_rotate_with_left( t );
            else
                t = double_rotate_with_left( t );
        }
    }
    else if( e > t->data )
    {
        t->right = insertTree( e, t->right );
        if( height( t->right ) - height( t->left ) == 2 ){
            if( e > t->right->data )
                t = single_rotate_with_right( t );
            else
                t = double_rotate_with_right( t );
        }
    }

    t->height = max( height( t->left ), height( t->right ) ) + 1;
    return t;
}

Node *insertTree1(localization e, Node1 *t)
{
    if( t == NULL )
    {
        t = (Node1*)malloc(sizeof(Node1));
        if( t == NULL )
        {
            fprintf (stderr, "Out of memory!!! (insert)\n");
            exit(1);
        }
        else
        {
            t->data = e;
            t->height = 0;
            t->left = t->right = NULL;
        }
    }
    else if( checkPriority(e,t->data)==0 )
    {
        t->left = insertTree1( e, t->left );
        if( height1( t->left ) - height1( t->right ) == 2 )
            if( checkPriority(e,t->left->data)==0 )
                t = single_rotate_with_left( t );
            else
                t = double_rotate_with_left( t );
    }
    else if( checkPriority(e,t->data)!=0 )
    {
        t->right = insertTree1( e, t->right );
        if( height1( t->right ) - height1( t->left ) == 2 )
            if( checkPriority(e,t->right->data)!=0 )
                t = single_rotate_with_right( t );
            else
                t = double_rotate_with_right( t );
    }

    t->height = max( height1( t->left ), height1( t->right ) ) + 1;
    return t;
}



Node *minValueNode(Node *node)
{
    Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

Node* deleteNode(Node* root, int key)
{
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
