struct lightbulb{
char code[5];
int watts;
int type;
int cant;
int active; //deletion is implemented in logical way
} ;

void createBulb(struct lightbulb *bulb){
     printf("Enter lightbulb code (max 5 chars). \n");
     scanf("%s",bulb->code);
     printf("Enter lightbulb watts.\n");
     scanf("%d",&bulb->watts);
     printf("Enter lightbulb type. \n");
     scanf("%d",&bulb->type);
     printf("Enter lightbulb stock. \n");
     scanf("%d",&bulb->cant);
    bulb->active=1;
}
