typedef struct {
    char name[40];
    char lastname[40];
}author_structure;

typedef struct {
    char name[40];
    int code;
     author_structure author;
    char category[40];
    int taken;
    }book_structure;

typedef struct {
    char student[40];
    book_structure *book;
}loan_structure;



void createBook( book_structure *book){
     printf("Ingrese el nombre del libro sin espacios(max 40 caracteres)\n");
     scanf("%s",book->name);
     printf("Ingrese el codigo del libro\n");
     scanf("%d",&book->code);
     printf("Ingrese el nombre del autor(max 40 caracteres) \n");
     scanf("%s",book->author.name);
     printf("Ingrese el apellido del autor(max 40 caracteres) \n");
     scanf("%s",book->author.lastname);
     printf("Ingrese la categoria del libro(max 40 caracteres) \n");
     scanf("%s",book->category);
     book->taken=0;
}


