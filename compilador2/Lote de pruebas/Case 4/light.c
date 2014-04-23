#include<stdlib.h>
#include<stdio.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 4\lightbulb.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 4\list.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 4\tree.h"






void main()
{
    node *root;
    List mylist;
    initlist(&mylist);
    root = NULL;
    int n=length(mylist);
    int i=0;
    for(i;i<n;i++){
        insert(&root,getitem(mylist,i));
    }
    int menu;
    do{printf("\n\n1- Add lightbulb \n2- Delete lightbulb \n3- Edit lightbulb \n4- Print tree\n5- Compress tree\n\n");
    scanf("%d",&menu);
    switch(menu){
    case 1: addbulb(&root); break;
    case 2: deletebulb(root); break;
    case 3: editbulb(root);break;
    case 4: printtree(root);break;
    case 5: compresstree(root);break;
    }}
     while(menu<6);
}

void addbulb(node ** root){
     lightbulb bulb;
    createBulb(&bulb);
    insert(root, bulb);

}

void deletebulb(node * root){
     lightbulb find;
    createBulb(&find);
    node *tmp;
    tmp = search(&root, find);
    if (tmp)
    {
        tmp->data.active=0;
        printf("Data succesfully deleted.\n");
    }
    else
    {
        printf("Data Not found in tree.\n");
    }

}

void editbulb(node * root){
     lightbulb find;
    createBulb(&find);
    node *tmp;
    tmp = search(&root, find);
    if (tmp)
    {
        printf("\nInsert new values\n");
        createBulb(&root->data);
        printf("Data succesfully edited.\n");
    }
    else
    {
        printf("Data Not found in tree.\n");
    }

}

void printtree(node *root){
    printf("Pre Order Display\n");
    print_preorder(root);

    printf("In Order Display\n");
    print_inorder(root);

    printf("Post Order Display\n");
    print_postorder(root);
}



void compresstree(node *root){
        node *newroot;
        newroot=NULL;
        compress(root,newroot);
        root=newroot;

}

void compress(node *tree,node *newroot){
    if (tree)
    {
        insert(&newroot,tree->data);
        compress(tree->left,newroot);
        compress(tree->right,newroot);

    }
}

