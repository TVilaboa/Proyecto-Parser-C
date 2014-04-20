typedef struct
{
     school data;
    struct node * right, * left;
}node ;


//insert
void insert(node ** tree,  school val)
{
    node *temp = NULL;
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        temp->data = val;
        *tree = temp;
        return;
    }

    if(val.code < (*tree)->data.code)
    {
        insert(&(*tree)->left, val);
    }
    else if(val.code > (*tree)->data.code)
    {
        insert(&(*tree)->right, val);
    }

}

void print_preorder(node * tree)
{
    if (tree)
    {
        printschool(tree);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}

void print_inorder(node * tree)
{
    if (tree)
    {
        print_inorder(tree->left);
        printschool(tree);
        print_inorder(tree->right);
    }
}

void print_postorder(node * tree)
{
    if (tree)
    {
        print_postorder(tree->left);
        print_postorder(tree->right);
        printschool(tree);
    }
}

void printschool(node * tree)
{

    printf("\nCode:%d\n",tree->data.code);
    printf("Ballot Boxes: %d\n",tree->data.cantboxes);


}

//delete whole tree
void deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

//search a school and returns it
node* search(node ** tree,  school val)
{
    if(!(*tree))
    {
        return NULL;
    }

    if(val.code < (*tree)->data.code)
    {
        search(&((*tree)->left), val);
    }
    else if(val.code > (*tree)->data.code)
    {
        search(&((*tree)->right), val);
    }
    else if(val.code == (*tree)->data.code)
    {
        return *tree;
    }
}
