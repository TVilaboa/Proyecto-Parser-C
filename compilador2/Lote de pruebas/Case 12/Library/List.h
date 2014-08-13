#ifndef _LIST_H
#define _LIST_H
typedef struct  {
void *data;
struct Listitem *next;
}Listitem;

typedef struct  {
Listitem *head;
}List;

void initlist (List *); /* initialize an empty list */
void insertfront(List * , void *val); /* insert val at front */
void insertback(List *, void *val); /* insert val at back */
int length(List); /* returns list length */
void destroy(List *); /* deletes list */
void setitem(List *, int n, void *val);/* modifies item at n to val*/
void *getitem(List, int n); /* returns value at n*/
#endif /* _LIST_H */

void initlist(List *ilist) {
ilist->head = 0;
}

void insertfront(List *ilist, void *val) {
Listitem *newitem;
newitem = (Listitem *)malloc(sizeof(Listitem));
newitem->next = ilist->head;
newitem->data = val;
ilist->head = newitem;
}

void insertback(List *ilist, void *val) {
Listitem *ptr;
Listitem *newitem;
newitem = (Listitem *)malloc(sizeof(Listitem));
newitem->data = val;
newitem->next = 0;
if (!ilist->head) {
ilist->head = newitem;
return;
}
ptr = ilist->head;
while (ptr->next)
{
ptr = ptr->next;
}
ptr->next = newitem;
}



int length(List ilist){ /* returns list length */
Listitem *ptr;
int count = 1;
if (!ilist.head) return 0;
ptr = ilist.head;
while (ptr->next) {
ptr = ptr->next;
count++;
}
return count;
}

void destroy(List *ilist) { /* deletes list */
Listitem *ptr1,*ptr2;
if (!ilist->head) return; /* nothing to destroy */
ptr1 = ilist->head; /* destroy one by one */
while (ptr1) {
ptr2 = ptr1;
ptr1 = ptr1->next;
free(ptr2);
}
ilist->head = 0;
}

void setitem(List *ilist, int n, void *val){
/* modifies a value*/
/* assume length is at least n long */
Listitem *ptr;
int count = 0;
if (!ilist->head) return;
ptr = ilist->head;
for (count = 0;count < n;count ++)
{
if (ptr) ptr = ptr->next;
else return;
}
if (ptr)
ptr->data = val;
}

void *getitem(List ilist, int n) {
/* returns a list value,
* assume length is at least n long */
Listitem *ptr;
int count = 0;
if (!ilist.head) return 0;
ptr = ilist.head;
if (n==0) return ptr->data;
while (ptr->next) {
ptr = ptr->next;
count++;
if (n == count)
return (ptr->data);
}
return 0;
}
