#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// celulele pentru banda
typedef struct Nod {
    char value;
    struct Nod *left, *right;
} nod;

// celulele pentru coada/stiva
typedef struct Cell {
    char *value;
    struct Cell *next;
} cell;

// banda
typedef struct Banda {
    nod *head, *finger;
} banda;

// coada
typedef struct Queue {
    cell *head, *tail;
    int length;
} queue;

typedef struct Stack {
    cell *head;
    int length;
}stack;


// alocare celula pentru coada
cell *initCell(char *value) {
    cell *p = (cell *)malloc(sizeof(cell));  // prima alocare e pentru cells

    // alocam memorie pentru sirul de caractere reprezentat de comanda
    p->value = (char *)malloc(
        sizeof(char) *
        (strlen(value) + 1));  // a doua alocare e pentru char *value
    strcpy(p->value, value);
    p->next = NULL;
    return p;
}

// void push(stack *st, char* value) {
//     cell *p = initCell(value);
//     if(st->head == NULL){
//         st->head = p;
//         st->length = 1;
//         return;
//     }
//     p->next = st->head;
//     st->head = p;
//     st->length ++;
//     return;
// }

// void pop(stack *st) {
//     cell *aux = st->head;
//     st->head = st->head->next;
//     free(aux);
//     st->length--;
// }

// char * top(stack *st) {
//     return st->head->value;
// }









// initializam coada
queue *initQ() {
    queue *q = (queue *)malloc(
        sizeof(queue));  //  a treia alocare e pentru coada in sine
    q->head = q->tail = NULL;
    q->length = 0;
    return q;
}

// verificam daca coada este goala
int emptyQ(queue *q) {
    if (q->head == NULL) return 1;
    return 0;
    // 1 = empty
}

// stergere din coada
void popQ(queue *q) {
    cell *u = q->head;
    q->head = u->next;
    free(u);
    --q->length;
}

// adaugare in coada(head, element, element, element, tail)
void addQ(queue *q, char *value) {
    cell *nou = initCell(value);  // apelul pentru prima si a doua
    if (!q->length) {
        q->head = nou;
        q->tail = nou;
    } else {
        q->tail->next = nou;
        q->tail = nou;
    }
    (q->length)++;
}

// creare santinela + prima pozitie
void init(banda *p) {
    // alocam memorie pentru santinela, prima pozitie
    p->head = (nod *)malloc(sizeof(nod));     // a 4-a alocare pentru head
    nod *first = (nod *)malloc(sizeof(nod));  // a 5-a alocare pentru first
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

// OPERATII DE TIP QUERY
// afisam banda cu tot cu deget
void show(banda *p) {
    // luam un nod auxiliar care sa primeasca headul listei
    nod *aux = p->head;
    printf("banda: ");
    while (aux != NULL) {
        if (aux == p->finger)
            printf("|%c|", aux->value);
        else
            printf("%c", aux->value);
        aux = aux->right;
    }
    printf("\n");
}

void show_current(banda *p) { printf("%c", p->finger->value); }

// OPERATII DE TIP UPDATE
// move_left move_right
void move(banda *a, char direction) {
    if (direction == 'l') {
        if (a->finger !=
            a->head
                ->right)  // daca nu suntem pe prima pozitie (head e santinela)
            a->finger = a->finger->left;
    }

    if (direction == 'r') {
        if (a->finger->right == NULL) {
            nod *aux = (nod *)malloc(
                sizeof(nod));  //  a 6-a alocare pentru nod nou in move
            aux->value = '#';  // DE MODIFICAT
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
    if (direction == 'l') {
        while (p->left != NULL && p->value != c) {
            p = p->left;
        }
        if (p->value != c)
            printf("ERROR");
        else
            a->finger = p;
    }

    if (direction == 'r') {
        while (p->right != NULL && p->value != c) {
            p = p->right;
        }

        if (p->value != c) {
            nod *aux = (nod *)malloc(
                sizeof(nod));  // a 7-a alocare pentru nod nou in move_char
            aux->value = '#';
            aux->right = NULL;
            p->right = aux;  //<=>a->head->right->right = aux;
            aux->left = p;
            p = p->right;  // mutam degetu pe pozitia aia
        }
    }
}

// write <c>
void write(banda *a, char c) { a->finger->value = c; }

// NETESTATA
// void insert_char(banda *a, char direction, char c) {
//     nod *p = a->finger;

//     if(direction == 'l') {
//         // nu uita ca head e santinela
//         if(p == a->head->right) {
//             printf("ERROR\n");
//         } else {
//             nod *u = a->finger->left;
//             nod *nou = (nod *)malloc(sizeof(nod));
//             nou->left = u;
//             nou->right = p;
//             nou->value = c;

//             u->right = nou;
//             p->left = nou;

//             a->finger = nou;
//         }
//     }

//     if(direction == 'r') {
//         if(a->finger->right){
//             nod *u = a->finger->right;
//             nod *nou = (nod *)malloc(sizeof(nod));
//             p->right = nou;
//             u->left = nou;
//             nou->value = c;
//             nou->left = p;
//             nou->right = u;
//             a->finger = nou;
//         } else {
//             nod *nou = (nod *)malloc(sizeof(nod));
//             p->right = nou;
//             nou->value = c;
//             nou->left = p;
//             a->finger = nou;
//         }
//     }
// }




int empty_banda(banda *a) {
    if (a->head == NULL) return 0;
    return 1;
}
// stergem din banda
void pop_banda(banda *a) {
    nod *u = a->head;
    a->head = u->right;  // mutam capul
    free(u);
}

// eliberam memoria structurilor
void delete_struct(queue *q, banda *a) {
    while (!emptyQ(q))
        popQ(q);  // in caz ca nu vi fi goala desi cred ca va fi mereu
    // sa scap de coada
    // printf("coada e coada?:%d\n", emptyQ(q));
    free(q);
    // dupa scapi de banda
    while (a->head != NULL) {
        pop_banda(a);  // eliberam simultan memoria pentru banda
        // show(a);
    }
}

int main() {
    // banda
    banda a;
    // coada de operatii
    queue *op;
    // numarul de operatii
    int t;
    // numele operatiilor
    char operation[20];
    char *o;

    // initializam coada de operatii si banda
    op = initQ();
    init(&a);
    show(&a);
    // citim numarul de operatii
    scanf("%d", &t);
    // scapi de enter
    getchar();

    for (int i = 1; i <= t; ++i) {
        fgets(operation, 20, stdin);
        // scapam de enter
        operation[strlen(operation) - 1] = '\0';

        // printf("operatie:%s\n", operation);
        if (strcmp(operation, "EXECUTE") == 0) {
            // scoatem din coada si facem operatia
            if (!emptyQ(op)) {
                o = op->head->value;
                popQ(op);
                if (strstr(o, "WRITE")) write(&a, o[strlen(o) - 1]);

                if (strcmp(o, "MOVE_RIGHT") == 0) move(&a, 'r');
                if (strcmp(o, "MOVE_LEFT") == 0) move(&a, 'l');
                // if(strcmp(o, "MOVE"))
            }
        } else {
            if (strcmp(operation, "UNDO") == 0) {
                printf("face undo\n");
            } else {
                if (strcmp(operation, "REDO") == 0) {
                    printf("face redo\n");
                } else {
                    if (strcmp(operation, "SHOW") == 0) {
                        show(&a);
                    } else {
                        if (strcmp(operation, "SHOW_CURRENT") == 0) {
                            show_current(&a);
                        }
                        // daca nu e execute sau undo/redo sau query
                        // atunci e
                        // bagat in coada
                        else {
                            addQ(op, operation);  // adaugat   in coada
                        }
                    }
                }
            }
        }

        printf("dupa operatie: ");
        show(&a);
    }

    delete_struct(op, &a);
    return 0;
}

// VALGRIND 1 eroare
// MAKEFILE de modificat pt teste
// UNDO, REDO se fac usor doar adaugi in stiva dupa fiecare move si golesti daca dai de write
// INSERT, ALEA CU MOVE_CHAR de testat

// la asta ai 1 memory leak pt exemplu
