typedef struct {
 lightbulb data;
struct node * right, * left;
}node;




void insert(node ** tree,  lightbulb val)
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

    if(val.type < (*tree)->data.type)
    {
        insert(&(*tree)->left, val);
    }
    else if(val.type > (*tree)->data.type)
    {
        insert(&(*tree)->right, val);
    }

}

void print_preorder(node * tree)
{
    if (tree)
    {
        printlightBulb(tree);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}

void print_inorder(node * tree)
{
    if (tree)
    {
        print_inorder(tree->left);
        printlightBulb(tree);
        print_inorder(tree->right);
    }
}

void print_postorder(node * tree)
{
    if (tree)
    {
        print_postorder(tree->left);
        print_postorder(tree->right);
        printlightBulb(tree);
    }
}

void printlightBulb(node * tree){
    if(tree->data.active==1){
    printf("\nCode:%s\n",tree->data.code);
        printf("Watts:%d\n",tree->data.watts);
        printf("Type:%d\n",tree->data.type);
        printf("Stock:%d\n\n",tree->data.cant);
    }
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

node* search(node ** tree,  lightbulb val)
{
    if(!(*tree))
    {
        return NULL;
    }

    if(val.type < (*tree)->data.type)
    {
        search(&((*tree)->left), val);
    }
    else if(val.type > (*tree)->data.type)
    {
        search(&((*tree)->right), val);
    }
    else if(val.type == (*tree)->data.type)
    {
        return *tree;
    }
}
