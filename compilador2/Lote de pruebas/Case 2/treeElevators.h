struct bin_tree {
struct call data;
struct bin_tree * right, * left;
};
typedef struct bin_tree node;

void insert(node ** tree, struct call val)
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

    if(val.floor < (*tree)->data.floor)
    {
        insert(&(*tree)->left, val);
    }
    else if(val.floor > (*tree)->data.floor)
    {
        insert(&(*tree)->right, val);
    }

}

void print_preorder(node * tree)
{
    if (tree)
    {
        printcall(tree);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}

void print_inorder(node * tree)
{
    if (tree)
    {
        print_inorder(tree->left);
        printcall(tree);
        print_inorder(tree->right);
    }
}

void print_postorder(node * tree)
{
    if (tree)
    {
        print_postorder(tree->left);
        print_postorder(tree->right);
        printcall(tree);
    }
}

void printcall(node * tree){

        printf("Floor:%d\n",tree->data.floor);
        printf("TotalDelay:%d\n\n",tree->data.delay);

}

void deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

//search and return a call
node* search(node ** tree, struct call val)
{
    if(!(*tree))
    {
        return NULL;
    }

    if(val.floor < (*tree)->data.floor)
    {
        search(&((*tree)->left), val);
    }
    else if(val.floor > (*tree)->data.floor)
    {
        search(&((*tree)->right), val);
    }
    else if(val.floor == (*tree)->data.floor)
    {
        return *tree;
    }
}
