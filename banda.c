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
    cell *front, *tail;
    int length;
} queue;

typedef struct Stack {
    cell *head;
    int length;
} stack;

// alocam memorie pentru cell(element al cozii/stivei)
cell *initCell(char *value) {
    cell *p = (cell *)malloc(sizeof(cell));
    // alocam memorie pentru sirul de caractere reprezentat de comanda
    p->value = (char *)malloc(
        sizeof(char) *
        (strlen(value) + 1));  // a doua alocare e pentru char *value

    // copiem sirul in celula
    strcpy(p->value, value);
    p->next = NULL;
    return p;
}

// initializam coada
queue *initQ() {
    queue *q = (queue *)malloc(
        sizeof(queue));  //  a treia alocare e pentru coada in sine
    q->front = q->tail = NULL;
    q->length = 0;
    return q;
}

// verificam daca coada este goala
int emptyQ(queue *q) {
    if (q->front == NULL) return 1;
    return 0;
    // 1 = empty
}

// stergem din coada
void popQ(queue *q) {
    cell *u = q->front;
    q->front = u->next;
    // stergem char* alocat pentru campul value
    free(u->value);
    free(u);
    --q->length;
}

// adaugam in coada(head, element, element, element, tail)
void addQ(queue *q, char *value) {
    // printf("in coada ");
    cell *nou = initCell(value);

    if (!q->length) {
        q->front = nou;
        q->tail = nou;
    } else {
        q->tail->next = nou;
        q->tail = nou;
    }
    (q->length)++;
}

// eliberam memoria ocupata de coada
void delete_queue(queue *q) {
    while (!emptyQ(q)) {
        popQ(q);
    }
    // eliberam memoria pentru structura in sine
    free(q);
}

// initializam stiva
stack *initStack() {
    stack *st = (stack *)malloc(sizeof(stack));
    st->head = NULL;
    st->length = 0;
    return st;
}

// adaugam in stiva
void pushS(stack *st, char *value) {
    // printf("in stiva ");
    cell *p = initCell(value);
    if (st->head == NULL) {
        st->head = p;
        st->length = 1;
        return;
    }
    p->next = st->head;
    st->head = p;
    st->length++;
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
char *topS(stack *st) { return st->head->value; }

// verificam daca stiva e goala
int emptyS(stack *st) {
    if (st->head == NULL) return 1;
    return 0;
    // 1 = empty
}

// eliberam memoria ocupata de stiva
void delete_stack(stack *st) {
    while (!emptyS(st)) {
        popS(st);
    }
    free(st);
}

// alocam memorie pentru nod(element al benzii)
nod *initNod(char value) {
    nod *p = (nod *)malloc(sizeof(nod));  // prima alocare e pentru cells
    p->value = value;
    // printf("s-a adaugat operatia %c\n", p->value);
    p->left = p->right = NULL;
    return p;
}

// cream santinela + prima pozitie
banda *initBanda() {
    // alocam memorie pentru banda
    banda *b = (banda *)malloc(sizeof(banda));
    b->finger = b->head = NULL;

    // punem valoarea santinelei pe '?' si # pe prima pozitie
    b->head = initNod('?');
    nod *first = initNod('#');

    // facem cele 2 legaturi(celelalte 2 sunt nule de la initNod)
    b->head->right = first;
    first->left = b->head;

    // punem fingerul pe prima pozitie a benzii
    b->finger = first;
    return b;
}

// cu tot cu santinela?????????????????????????
void afisam_banda(banda *b) {
    nod *aux = b->head;
    while (aux != NULL) {
        printf("%c ", aux->value);
        aux = aux->right;
    }
    printf("\n");
}

// verificam daca banda contine doar santinela
int empty_banda(banda *b) {
    // verificam daca e null elementul de dupa santinela
    if (b->head->right == NULL) return 1;
    return 0;
}

// stergem din banda
void pop_banda(banda *b) {
    nod *u = b->head;         // santinela
    nod *v = b->head->right;  // primul element al benzii
    // printf("primul element al benzii:%c\n", v->value);

    // daca e un sg element in banda
    if (v->right == NULL) {
        u->right = NULL;  // dupa stergere avem dupa santinela NULL
    } else {
        u->right = v->right;  // de la stanga spre dreapta
        v->right->left = u;   // de la dreapta spre stanga
    }
    free(v);  // eliberam memoria
}

// eliberam memoria ocupata de banda
void delete_banda(banda *b) {
    while (!empty_banda(b)) {
        pop_banda(b);
    }
    free(b->head);  // santinela
    free(b);        // structura in sine
}

// move_left move_right
void move(banda *b, char direction) {
    if (direction == 'l') {
        if (b->finger != b->head->right)  // daca nu suntem pe prima pozitie
            b->finger = b->finger->left;  // putem trece o pozitie la stanga
        // altfel fingerul ramane pe aceeasi pozitie
    }

    if (direction == 'r') {
        if (b->finger->right == NULL) {  // daca suntem pe ultima pozitie
            nod *aux = initNod('#');     // cream inca un nod
            b->finger->right = aux;      //<=>a->head->right->right = aux;
            aux->left = b->finger;
        }
        b->finger = b->finger->right;  // mutam degetu pe pozitia aia
    }
}

// show
void show(banda *b, FILE *out) {
    // luam un nod auxiliar care sa primeasca primul element al listei
    nod *aux = b->head->right;
    // printf("banda: ");
    while (aux != NULL) {
        if (aux == b->finger) {
            fprintf(out, "|%c|", aux->value);
            // printf("|%c|", aux->value);
        } else {
            fprintf(out, "%c", aux->value);
            // printf("%c", aux->value);
        }
        aux = aux->right;
    }
    fprintf(out, "\n");
    // printf("\n");
}

// o sa o stergi
void show_consola(banda *b) {
    // luam un nod auxiliar care sa primeasca primul element al listei
    nod *aux = b->head->right;
    // printf("banda: ");
    while (aux != NULL) {
        if (aux == b->finger) {
             printf("|%c|", aux->value);
        } else {
            printf("%c", aux->value);
        }
        aux = aux->right;
    }
    printf("\n");
}

// write <c> 
void write(banda *b, char c) { b->finger->value = c; }

// show_current
void show_current(banda *b, FILE *out) {
    fprintf(out, "%c\n", b->finger->value);
    // printf("%c\n", b->finger->value);
}

// insert_char
void insert_char(banda *b, char direction, char c, FILE *out) {
    nod *v = b->finger;
    // printf("caracterul de bagat este %c\n", c);
    if (direction == 'l') {
        // nu uita ca head e santinela
        if (v == b->head->right) {
            // daca e fix pe prima pozitie
            //printf("ERROR\n");
            fprintf(out, "ERROR\n");
        } else {
            nod *u = b->finger->left;
            // alocam memorie pentru nodul nou
            nod *nou = initNod(c);
            // facem legaturile cu u si v astfel incat sa avem u nou v 
            u->right = nou;
            nou->left = u;
            v->left = nou;
            nou->right = v;
            // actualizam fingerul
            b->finger = nou;
        }
    }

    if (direction == 'r') {
        nod *nou = initNod(c);
        if (b->finger->right) { // v nou w
            nod *w = b->finger->right;
            v->right = nou;
            nou->left = v;
            w->left = nou;
            nou->right = w;            
        } else {
            v->right = nou;
            nou->left = v;
        }

        b->finger = nou;
    }
}

int main() {
    FILE *in = fopen("tema1.in", "r");
    FILE *out = fopen("tema1.out", "w");

    banda *b;
    stack *st;
    queue *q;

    b = initBanda();
    st = initStack();
    q = initQ();

    // move(b, 'r');
    // show(b, out);
    // move(b, 'r');
    // show(b, out);
    // move(b, 'r');
    // show(b, out);
    // move(b, 'r');
    // show(b, out);
    // move(b, 'l');
    // show(b, out);
    // move(b, 'l');
    // show(b, out);
    // move(b, 'l');
    // show(b, out);
    // move(b, 'l');
    // show(b, out);
    // move(b, 'l');
    // show(b, out);
    // write(b, 'A');
    // show(b, out);

    int t;
    char operation[100];
    char *o;

    fscanf(in, "%d", &t);
    fgets(operation, 100, in);  // sa scapam de enterul de dupa t

    for (int i = 1; i <= t; ++i) {
        fgets(operation, 100, in);
        // stergem enterul retinut de fgets
        operation[strlen(operation) - 1] = '\0';
        if (strcmp(operation, "EXECUTE") == 0) {
            o = q->front->value;  // FII ATENT CA TU PASTREZI ADRESA AICI, NU
                                  // VALOAREA ASA CA POPUL TREBUIE FACUT in toate ifurile
            if (strstr(o, "WRITE")) {
                // printf("operation:%s\n", o);
                // printf("strlen:%ld\n", strlen(o));
                // printf("trebuie adaugat caracterul:%c\n", o[strlen(o)-1]);
                write(b, o[strlen(o) - 1]);
                // printf("banda dupa:");
                // show_consola(b);
                popQ(q);
                continue;
            }

            if (strstr(o, "MOVE_RIGHT") && !strstr(o, "char")) {
                move(b, 'r');
                // printf("banda dupa:");
                // show_consola(b);
                popQ(q);
                continue;
            }

            if (strstr(o, "MOVE_LEFT") && !strstr(o, "char")) {
                move(b, 'l');
                // printf("banda dupa:");
                // show_consola(b);
                popQ(q);
                continue;
            }

            if (strstr(o, "INSERT_RIGHT")) {
                // printf("operation:%s\n", o);
                // printf("trebuie adaugat caracterul:%c\n", o[strlen(o)-1]);
                insert_char(b, 'r', o[strlen(o) - 1], out);
                // printf("banda dupa:");
                // show_consola(b);
                popQ(q);
                continue;
            }

            if (strstr(o, "INSERT_LEFT")){
                // printf("operation:%s\n", o);
                // printf("trebuie adaugat caracterul:%c\n", o[strlen(o)-1]);
                insert_char(b, 'l', o[strlen(o) - 1], out);
                // printf("banda dupa:");
                // show_consola(b);
                popQ(q);
                continue;
            }

            // if (strstr(o, "MOVE_LEFT_CHAR"))
            //  move_char(&a, 'l', o[15], out);
            //  if (strstr(o, "MOVE_RIGHT_CHAR"))
            //  move_char(&a, 'r', o[16], out);
            
            continue;
        }

        if (strcmp(operation, "UNDO") == 0) {
            // printf("face undo\n");
            // printf("banda dupa:");
            //     show_consola(b);
            continue;
        }

        if (strcmp(operation, "REDO") == 0) {
            // printf("face redo\n");
            //printf("banda dupa:");
                //show_consola(b);
            continue;
        }

        if (strcmp(operation, "SHOW") == 0) {
            // printf("face show:");
            // printf("banda dupa:");
            //     show_consola(b);
            show(b, out);
            continue;
        }

        if (strcmp(operation, "SHOW_CURRENT") == 0) {
            // printf("face show_curent:");
            // printf("banda dupa:");
            // show_consola(b);
            show_current(b, out);
            // printf("\n");
            continue;
        }
        addQ(q, operation);
        //printf("banda dupa:");
                //show_consola(b);
        // printf("COADA MATIIII\n");
    }

    delete_queue(q);
    delete_stack(st);
    delete_banda(b);

    fclose(in);
    fclose(out);
    return 0;
}
// // UNDO, REDO se fac usor doar adaugi in stiva dupa fiecare move si golesti
// daca
// // dai de write INSERT, ALEA CU MOVE_CHAR de testat

// // OPERATII DE TIP QUERY
// // afisam banda cu tot cu deget

// // OPERATII DE TIP UPDATE

// // move_left_char move_right_char
// void move_char(banda *a, char direction, char c, FILE *out) {
//     nod *p = a->finger;
//     if (direction == 'l') {
//         while (p->left != NULL && p->value != c) {
//             p = p->left;
//         }
//         if (p->value != c)
//         {
//             //fprintf(out, "ERROR\n");
//             printf("ERROR\n");
//         }
//         else
//             a->finger = p;
//     }

//     if (direction == 'r') {
//         while (p->right != NULL && p->value != c) {
//             p = p->right;
//         }

//         if (p->value != c) {
//             nod *aux = (nod *)malloc(
//                 sizeof(nod));  // a 7-a alocare pentru nod nou in move_char
//             aux->value = '#';
//             aux->right = NULL;
//             p->right = aux;  //<=>a->head->right->right = aux;
//             aux->left = p;
//             p = p->right;  // mutam degetu pe pozitia aia
//         }
//     }
// }


// // OPERATII DE TIP UNDO/REDO
// // void undo(banda *a, stack *undo_st, stack *redo_st) {
// //     push(redo_st, top(undo_st));
// //     if(strstr(top(undo_st), "RIGHT"))
// //         a->finger
// // }
