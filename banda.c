#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// celulele pentru banda
typedef struct Nod
{
    char value;
    struct Nod *left, *right;
} nod;

// celulele pentru coada/stiva
typedef struct Cell
{
    char *value;
    struct Cell *next;
} cell;

// banda
typedef struct Banda
{
    nod *head, *finger;
} banda;

// coada
typedef struct Queue
{
    cell *head, *tail;
    int length;
} queue;

typedef struct Stack
{
    cell *head;
    int length;
} stack;

// alocam memorie pentru cell(element al cozii/stivei)
cell *initCell(char *value)
{
    cell *p = (cell *)malloc(sizeof(cell)); // prima alocare e pentru cells
    printf("%ld\n", strlen(value));
    // alocam memorie pentru sirul de caractere reprezentat de comanda
    p->value = (char *)malloc(sizeof(char) * (strlen(value) + 1)); // a doua alocare e pentru char *value

    // copiem sirul in celula
    strcpy(p->value, value);
    printf("s-a adaugat operatia %s\n", p->value);
    p->next = NULL;
    return p;
}

// initializam coada
queue *initQ()
{
    queue *q = (queue *)malloc(
        sizeof(queue)); //  a treia alocare e pentru coada in sine
    q->head = q->tail = NULL;
    q->length = 0;
    return q;
}

// verificam daca coada este goala
int emptyQ(queue *q)
{
    if (q->head == NULL)
        return 1;
    return 0;
    // 1 = empty
}

// stergere din coada
void popQ(queue *q)
{
    cell *u = q->head;
    q->head = u->next;
    // stergem char* alocat pentru campul value
    free(u->value);
    free(u);
    --q->length;
}

// adaugare in coada(head, element, element, element, tail)
void addQ(queue *q, char *value)
{
    printf("in coada ");
    cell *nou = initCell(value);

    if (!q->length)
    {
        q->head = nou;
        q->tail = nou;
    }
    else
    {
        q->tail->next = nou;
        q->tail = nou;
    }
    (q->length)++;
}

// eliberam memoria ocupata de coada
void delete_queue(queue *q)
{
    while (!emptyQ(q))
    {
        popQ(q);
    }
    // eliberam memoria pentru structura in sine
    free(q);
}





// initializam stiva
stack *initStack() {
    stack *st = (stack *)malloc(
        sizeof(stack)); 
    st->head  = NULL;
    st->length = 0;
    return st;
}

// adaugam in stiva
void pushS(stack *st, char* value) {
    printf("in stiva ");
    cell *p = initCell(value);
    if(st->head == NULL){
        st->head = p;
        st->length = 1;
        return;
    }
    p->next = st->head;
    st->head = p;
    st->length ++;
    return;
}

// stergem din stiva
void popS(stack *st) {
    cell *aux = st->head;
    st->head = st->head->next;
    free(aux->value);
    free(aux);
    st->length--;
}

// extragem varful stivei
char * topS(stack *st) {
    return st->head->value;
}

int emptyS(stack *st) {
    if(st -> head == NULL)
        return 1;
    return 0;
    // 1 = empty
}
void delete_stack(stack *st) {
    while(!emptyS(st)) {
        popS(st);
    }
    free(st);
}


// // creare santinela + prima pozitie
// void init(banda *p) {
//     // alocam memorie pentru santinela, prima pozitie
//     p->head = (nod *)malloc(sizeof(nod));     // a 4-a alocare pentru head
//     nod *first = (nod *)malloc(sizeof(nod));  // a 5-a alocare pentru first
//     p->finger = first;
//     // punem valueueoarea santinelei pe NULL si # pe prima pozitie
//     p->head->value = '?';//las asta aici
//     first->value = '#';

//     // facem cele 4 legaturi
//     p->head->right = first;
//     p->head->left = NULL;
//     first->right = NULL;
//     first->left = p->head;
// }

int main()
{
    FILE *in = fopen("t1.in", "r");
    FILE *out = fopen("t1.out", "w");
    //  banda
    // banda a;
    stack *st;
    st = initStack();
    // coada de operatii
    queue *q;
    q = initQ();
    // numarul de operatii
    int t;
    // numele operatiilor
    char operation[100];
    // char *o;

    fscanf(in, "%d", &t);
    fgets(operation, 100, in); // sa scapam de enterul de dupa t
    
    for (int i = 1; i <= t; ++i)
    {
        fgets(operation, 100, in);
        // stergem enterul retinut de fgets
        operation[strlen(operation)-1] = '\0';
        addQ(q, operation);
        pushS(st, operation);
    }
    printf("topul stivei este %s\n", topS(st));
    printf("headul cozii este %s\n", q->head->value);
    delete_queue(q);
    delete_stack(st);
    fclose(in);
    fclose(out);
    return 0;
}
