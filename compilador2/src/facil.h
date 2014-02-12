    /* Biblioteca facil.h hecha por Pedro Cardoso Rodriguez
					      Ultima actualizacion 05-Abril-2005
 Esta libreria contiene 10 funciones para saber como hacer la llamada a cualquiera
de ellas ver su respectiva descripcion, que se encuentra dentro del cuerpo de
cada una de ellas. Estan en el orden de la declaracion */
#include <string.h> // Manejo de caracteres
#include <conio.h> // Uso del getch
#include <stdio.h> // Uso de cprintf y cscanf
#include <ctype.h> // Conversion de mayusculas-minusculas
#include <stdlib.h> // Conversion de tipos de datos
#include <dos.h> // Obtener horas fechas...
#include <time.h> // Uso de variables de tiempo
#include <bios.h> // Tambien pa algo del tiempo
#include <math.h> // Tambien pa algo del tiempo
#define di cprintf // Definiciones de comandos
#define ve gotoxy // Definiciones de comandos
#define limcad 151 // Una constante para la funcion lcad
		  // Funciones para formatos y aspectos en pantalla
void cuadro(int x1, int y1, int x2, int y2, char t, int borra);//Dibuja un cuadro
void limpia(int x1, int y1, int x2, int y2);//Limpia un area de la pantalla
void presentacion1(void); //Hace una presentacion en pantalla
				// Funciones para matrices
void ordenAR_1D(int AR[], int modo, int largo); // Ordena un arreglo unidimensional
		      // Funciones para cadenas
void centra(char txt[], int y);  //Centra una cadena de texto en un reglon
int lcad(int largo, int x, int y, char cuad, char cad[limcad], char ocultar);
               // Lee una cadena sin dejar que se pase de la longitud de la misma
		  // Otras funciones utiles
int respuesta(void); //Dice una decision del usuario de una pregunta de S/N
void waitMe(int); // Hace una pausa de n centesimas de segundos (como el delay de turbo c pero funcion en borland c)
int decAbin(int x); //Convierte de decimal a binario
int lint(int, int, int); // Lee un entero a prueba de tontos (no acepta letras o decimales)
			      // Ahora las funciones en si
void cuadro(int x1, int y1, int x2, int y2, char t, int borra)
{ int sqSiz, sqSder, sqIiz, sqIder, vert, horiz, temp;
  if((x1<1||x1>=x2)||(y1<1||y1>=y2)||(x2<=x1||x2>80)||(y2<=y1||y2>25)||(t!='s'&&t!='d'))
  { di("Hay un error en los argumentos");  return; }
  if(borra==1) clrscr();
  if(t=='s') sqSiz=218, sqSder=191, sqIiz=192, sqIder=217, vert=179, horiz=196;
  else sqSiz=201, sqSder=187, sqIiz=200, sqIder=188, vert=186, horiz=205;
  for(temp=x1;temp<x2;temp++) ve(temp,y1), di("%c",horiz);
  for(temp=x1;temp<x2;temp++) ve(temp,y2), di("%c",horiz);
  for(temp=y1;temp<y2;temp++) ve(x1,temp), di("%c",vert);
  for(temp=y1;temp<y2;temp++) ve(x2,temp), di("%c",vert);
  ve(x1,y1), di("%c",sqSiz), ve(x1,y2), di("%c",sqIiz);
  ve(x2,y2), di("%c",sqIder), ve(x2,y1), di("%c",sqSder); 
/* Este esta algo complicado, no, no es cierto. Para llamarlo se necesita: pa-
   sarle las coordendas en el orden: x1, x2, y1 y y2, luego una 's' para usar
   linea sencilla o una 'd' para doble (encerradas en comilla y minusculas) por
   ultimo, un 1 si queremos que borre la pantalla o cualquier otro valor para
   que no lo haga y solo dibuje el cuadro sobre cualquier pantalla anterior.
   si imprime mensaje de error el rango de valores es invalido o algun otro
   argumento, en cuyo hay que analizarlo (¡!)                              */
}

void limpia(int x1, int y1, int x2, int y2)
{  int ctdr, ctdr2;
	for(ctdr=y1;ctdr<=y2;ctdr++)
   	for(ctdr2=x1;ctdr2<=x2;ctdr2++) ve(ctdr2,ctdr), di("%c",32);
/* Para llamar esta funcion se le pasan los valores de coordenadas x1,y1 x2,y2
   ahora, dentro del cuadro formado por esas coordenadas sera borrado todo lo
   que este contenga ( limpia(2,2,45,3) por ejemplo )*/
}

void presentacion1(void) // Solo hay que hacer la llamada
{ int x1=19, y1=2, x2=59, y2=23, a, b, c, d, v, cr=177;
  clrscr();
  for(v=0;v<4;v++)
  { for(a=x1;a<x2;a++) ve(a,y1), di("%c",cr), waitMe(2); x2+=3;
    for(b=y1;b<y2;b++) ve(a,b), di("%c",cr), waitMe(2); y2-=2;
    for(c=a;c>x1;c--) ve(c,b), di("%c",cr), waitMe(2); x1-=3;
    for(d=b;d>y1;d--) ve(c,d), di("%c",cr), waitMe(2); y1+=2;
  }
  centra("Universidad Autonoma de Zacatecas",10);
  centra("Unidad Academica de Ingenieria",11);
  centra("Programa de Ingenieria en computacion",12);
  centra("Pedro Cardoso Rodriguez",13);
  centra("Grupo 2do \"C\"",14);
  centra("Presiona cualquier tecla para continuar...",24), getch(), clrscr();
}

void ordenAR_1D(int AR[], int modo, int largo)
{  int control, ind, x, y;
   do
   { control=0;
     for(ind=0;ind<largo;ind++)
     { x=AR[ind];
       if(ind<(largo-1)) y=AR[ind+1]; else y=x;
       if(x>y && modo==0) AR[ind]=y, AR[ind+1]=x, control=1;
       if(x<y && modo==1) AR[ind]=y, AR[ind+1]=x, control=1;
     }
   } while(control!=0);
/*  Al llamar se le pasa: a)Nombre del arreglo unidimensional sin corchetes,
    b)El 0 si queremos ordenar creciente, 1 para decreciente
    c)Numero de elementos de la matriz    */
}

void centra(char txt[], int y)
{  int x, longitud;
   if(y>25) ve(25,25), di("Error en el segundo argumento (fuera de rango)");
   else longitud=strlen(txt),  x=(80-longitud)/2, ve(x,y), di("%s",txt);
/* Para llamar esta funcion se le pasa la cadena de texto y el numero
   de reglon el cual debe ser  menor a 25 */
}

int lcad(int largo, int x, int y, char cuad, char cad[limcad], char ocultar)
{ char letra;
  int p;
  if(largo>limcad) { di("Error en argumentos"); return(0); }
  if(cuad=='s') limpia(x,y,(x+largo),y), cuadro(x-1,y-1,x+(largo-1),y+1,'s',2);
  ve(x,y), p=0, fflush(stdin);
  do
  { lee:
    { if(ocultar=='n') fflush(stdin), letra=getche();
      else if(ocultar=='s') fflush(stdin), letra=getch(), di("%c",6);
    }
    if(letra==0) { letra=getch(), printf("\b"), di("%c",32), printf("\b"); goto lee; }
    else if(letra!=13 && letra!=27 && letra!=8 && letra!=9) cad[p]=letra;
    else if(letra==9 && ocultar=='n')
      cad[p]=32, cad[p+1]=32, cad[p+2]=32, printf("\b"),di("%c%c%c",32,32,32), p+=2;
    else if(letra==9 && ocultar=='s')
      printf("\b"), di("%c",32), printf("\b"), p--;
    else if(letra==8 && p==0 && ocultar=='n') di("%c",179), p--;
    else if(letra==8 && p==0 && ocultar=='s')
      printf("\b"), di("%c",32), printf("\b"), p--;
    else if(letra==8 && p>0 && ocultar=='n')
       p--, cad[p]='\0', di("%c",32), printf("\b"), p--;
    else if(letra==8 && p>0 && ocultar=='s' && p!=(largo-1))
       p--, cad[p]='\0', printf("\b\b"),di("%c%c",32,32),printf("\b\b"), p--;
    else if(letra==8 && ocultar=='s' && p==(largo-1))
       printf("\b\b"), di("%c%c",32,179), printf("\b\b"), p-=2;
    else if(letra==27)
      { printf("\b"),di("%c",32),printf("\b"), cad[p]='\0'; return(0); }
    else if(letra==13)
      { printf("\b"),di("%c",32),printf("\b"), cad[p]='\0'; return(0); }
    p++;
    if(p==(largo-1)) cad[p]='\0', printf("\a");
    if(p>(largo-1))
      printf("\a\b"), di("%c",179), printf("\b"), p=(largo-1), cad[p]='\0';
  }
  while(p!=-5);
  return(0);
  /* Al llamar se le pasa: el largo de la cadena a guardar; la coordenada x y
     la y desde donde comenzara a leer; una s como caracter para si queremos
     que dibuje un marco que limite visualmente el largo de la cadena; la
     cadena donde se va a guardar.*/
}

int respuesta(void)
{  int res;
   do res=tolower(getch());
   while(res!='s' && res!='n');
   if(res=='s') return(1);
	else return(0);
/*Al llamar la funcion se supone que en pantalla esta la pregunta que el usuario
va a responder, si dice si, regresara 1, de lo contrario regresara un 0 */
}

void waitMe(int centsOfSec)
{  struct time act; // Al llamar la funcion se le pasa el numero de centesimas
   long double Ini, Fin; // de segundos que ha de durar la pausa tío
   gettime(&act), Ini=(((act.ti_min*60)*100)+(act.ti_sec*100)+(act.ti_hund));
   Fin=(Ini+centsOfSec);
   do gettime(&act), Ini=(((act.ti_min*60)*100)+(act.ti_sec*100)+(act.ti_hund));
   while(Ini<=Fin);
}

int decAbin(int x) // Al llamar pasarle un entero regresa como entero el binario
{  int tmp, bin;
   char tmpc[30]="", res[30]="";
   do
   { tmp=x%2, itoa(tmp,tmpc,2), strcat(res,tmpc), x=x/2;
   } while(x!=0);
   strrev(res), bin=atoi(res);
   return(bin);
}

int lint(int x, int y, int forma)
{ char c, aux[100];
  int cnt, val;
  if(x<1||x>80||y<1||y>25||forma<-1)
    { di("\aError en argumentos (lint)"); return(0); }
  do
  { cnt=0, val=1, ve(x,y), c='0';
    while(c!=13)
    { c=getch();
      if(c!=8&&c!=13) di("%c",c);
      else if(c==8&&cnt>0) printf("\b%c\b",32);
      if((c<'0'||c>'9')&&(c!=13&&c!=8)) val=0;
      if(c!=13&&c!=8) aux[cnt]=c, cnt++;
      if(cnt==12)
      ve(x,y), clreol(), di("\a%cNo juegue%c",173,33), cnt=0, getch(), ve(x,y), clreol();
      else if(c==8&&cnt>0) aux[cnt-1]='\0', cnt--;
      else if(c==13) aux[cnt]='\0';
    }
    if(val==1) cnt=atoi(aux);
    else
    { if(forma==-1) limpia(1,y,80,y);
      else if(forma==0) ve(x,y), clreol();
      else limpia(x,y,x+forma,y);
    }
  } while(val!=1);
  return(cnt);
  /* Al llamarla se le pasa el reglon y la fila donde va a leer, se le pasa la
forma: -1 para que borre todo el reglon si se equivocan, 0 para un clreol y un
numero mayor a 0 para borre esos espacios apartir del origen. Cuando el numero
sea valido lo regresa como int*/
} 