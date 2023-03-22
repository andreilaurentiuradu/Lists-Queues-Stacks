#include <stdio.h>
#include <stdlib.h>

typedef struct Nod {
    char value;
    struct Nod *left, *right;
} nod;

typedef struct banda {
    nod *head, *finger;
} banda;

typedef struct Coada {
    struct Coada *next;
    char *value;
} coada;

/// MERGE
// initializare coada(lista simplu inlantuita cu santinela)
void initQ(coada **q) {
    (*q) = (coada *)malloc(sizeof(coada));
    (*q)->next = NULL;
    (*q)->value = "\0";
}

/// MERGEE
// verificare coada nula
int emptyQ(coada *q) {
    if (q->next != NULL) return 0;
    return 1;
    // 1 = empty
}

/// MERGEE
// adaugare in coada
void addQ(coada *q, char *x) {
    coada *aux = (coada *)malloc(sizeof(coada));
    aux->value = x;
    aux->next = NULL;
    while (q->next) {
        q = q->next;
    }
    q->next = aux;  // urmatorul element dupa santinela
}

// stergere din coada
void popQ(coada *q) {
    coada *u = q->next;

    // mergem cu 2 noduri prin lista
    while (u->next != NULL) {
        q = u;        // copie pentru nodul u
        u = u->next;  // trecem la urmatorul nod
    }
    // am ajuns la ultimul nod (retinut in u)
    // q retine penultimul nod
    q->next = NULL;
    free(u);
}

/// MERGEEE
// afisare coada
void showQ(coada *q) {
    printf("coada este: ");
    while (q) {
        printf("%s\n", q->value);
        q = q->next;
    }
    printf("\n");
}

/// MERGE
// creare santinela + prima pozitie
void init(banda *p) {
    // alocam memorie pentru santinela, prima pozitie si deget
    p->head = (nod *)malloc(sizeof(nod));
    nod *first = (nod *)malloc(sizeof(nod));
    p->finger = (nod *)malloc(sizeof(nod));
    p->finger = first;
    // punem valueueoarea santinelei pe NULL si # pe prima pozitie
    p->head->value = '\0';
    first->value = '#';

    // facem cele 4 legaturi
    p->head->right = first;
    p->head->left = NULL;
    first->right = NULL;
    first->left = p->head;
}

/// MERGEE
// afisam banda cu tot cu deget
void show(banda *p) {
    // luam un nod auxiliar care sa primeasca headul listei
    nod *aux = p->head;
    while (aux != NULL) {
        if (aux == p->finger)
            printf("|%c|", aux->value);
        else
            printf("%c", aux->value);
        aux = aux->right;
    }
    printf("\n");
}

// OPERATII DE TIP UPDATE(toate operatiile merg, STERGE CONTORUL)

// move_left move_right
char contor = '0';
void move(banda *a, char direction) {
    if (direction == 'l') {
        if (a->finger != a->head->right)  // daca nu suntem pe prima pozitie (head e santinela)
            a->finger = a->finger->left;
    }

    if (direction == 'r') {
        if (a->finger->right == NULL) {
            nod *aux = (nod *)malloc(sizeof(nod));
            aux->value = contor; //DE MODIFICAT
            aux->right = NULL;
            a->finger->right = aux;  //<=>a->head->right->right = aux;
            aux->left = a->finger;
            a->finger = a->finger->right;  // mutam degetu pe pozitia aia
        }
    }
}

// move_left_char move_right_char
void move_char(banda *a, char direction, char c) {
    nod *p = a->finger;
    if(direction == 'l') {
        while(p->left != NULL && p->value != c){
            p = p->left;
        }
        if(p ->value != c)
            printf("ERROR");
        else
            a->finger = p;
    }

    if(direction == 'r') {
        while(p->right != NULL && p->value != c){
            p = p->right;
        }

        if (p->value != c) {
            nod *aux = (nod *)malloc(sizeof(nod));
            aux->value = '#';
            aux->right = NULL;
            p->right = aux;  //<=>a->head->right->right = aux;
            aux->left = p;
            p = p->right;  // mutam degetu pe pozitia aia
        }
    }
}

// write <c>
void write(banda *a, char c) { 
    a->finger->value = c; 
}

int main() {
    banda a;
    init(&a);
    show(&a);
    move(&a, 'r');
    contor++;
    show(&a);
    move(&a, 'r');
    contor++;
    show(&a);
    move(&a, 'r');
    contor++;
    show(&a);
    write(&a, 'A');
    show(&a);
    move_char(&a, 'l', '0');
    write(&a, '5');
    show(&a);
    move_char(&a, 'r', 'B');
    show(&a);


    coada *q;
    initQ(&q);
    showQ(q);
    // printf("%d\n", emptyQ(q));
    // addQ(q, "DA");
    // writeQ(q);
    // printf("%d\n", emptyQ(q));
    // addQ(q, "MErge");
    // writeQ(q);
    // printf("%d\n", emptyQ(q));
    // addQ(q, "esti");
    // popQ(q);
    // writeQ(q);
    // printf("%d\n", emptyQ(q));
    // addQ(q, "ZEUUUUU");
    // writeQ(q);
    // printf("%d\n", emptyQ(q));
    // popQ(q);
    // writeQ(q);

    // while(!emptyQ(q))
    //     popQ(q);
    // free(q); // sa scap de coada
    // dupa scapi de banda
    return 0;
}
