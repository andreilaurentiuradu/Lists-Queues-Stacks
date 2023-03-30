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
    p->value = (char *)malloc(sizeof(char) *(strlen(value) + 1));  // a doua alocare e pentru char *value

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
    //printf("in coada ");
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
    //printf("in stiva ");
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
    //printf("s-a adaugat operatia %c\n", p->value);
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
    while(aux != NULL)
    {
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
    nod *u = b->head; // santinela
    nod *v = b->head->right;  // primul element al benzii
    //printf("primul element al benzii:%c\n", v->value);
    
    // daca e un sg element in banda
    if(v->right == NULL) {
        u->right = NULL; // dupa stergere avem dupa santinela NULL
    } else {
        u->right = v->right; // de la stanga spre dreapta
        v->right->left = u; // de la dreapta spre stanga
    }
    free(v); // eliberam memoria
}

// eliberam memoria ocupata de banda
void delete_banda(banda *b) {
    while (!empty_banda(b)) {
        pop_banda(b);
    }
    free(b->head);  // santinela
    free(b);        // structura in sine
}

// move_left move_right merge in consola
void move(banda *b, char direction) {
    if (direction == 'l') {
        if (b->finger != b->head->right)  // daca nu suntem pe prima pozitie
            b->finger = b->finger->left; // putem trece o pozitie la stanga
        // altfel fingerul ramane pe aceeasi pozitie
    }

    if (direction == 'r') {
        if (b->finger->right == NULL) { // daca suntem pe ultima pozitie
            nod *aux = initNod('#'); // cream inca un nod
            b->finger->right = aux;  //<=>a->head->right->right = aux;
            aux->left = b->finger;
            b->finger = b->finger->right;  // mutam degetu pe pozitia aia
        }
    }
}

// show merge in consola
void show(banda *b, FILE *out) {
    // luam un nod auxiliar care sa primeasca primul element al listei
    nod *aux = b->head->right;
    printf("banda: ");
    while (aux != NULL) {
        if (aux == b->finger) {
            // fprintf(out, "|%c|", aux->value);
            printf("|%c|", aux->value);
        } else {
            // fprintf(out, "%c", aux->value);
            printf("%c", aux->value);
        }
        aux = aux->right;
    }
    // fprintf(out, "\n");
    printf("\n");
}

int main() {
    FILE *in = fopen("t1.in", "r");
    FILE *out = fopen("t1.out", "w");

    banda *b;
    stack *st;
    queue *q;

    b = initBanda();
    st = initStack();
    q = initQ();

    int t;
    char operation[100];
    // char *o;

    fscanf(in, "%d", &t);
    fgets(operation, 100, in);  // sa scapam de enterul de dupa t

    for (int i = 1; i <= t; ++i) {
        fgets(operation, 100, in);
        // stergem enterul retinut de fgets
        operation[strlen(operation) - 1] = '\0';
        addQ(q, operation);
        pushS(st, operation);
    }

    move(b, 'r');
    show(b, out);
    move(b, 'r');
    show(b, out);
    move(b, 'r');
    show(b, out);
    move(b, 'r');
    show(b, out);
    move(b, 'l');
    show(b, out);
    move(b, 'l');
    show(b, out);
    move(b, 'l');
    show(b, out);
    move(b, 'l');
    show(b, out);
    move(b, 'l');
    show(b, out);

    delete_queue(q);
    delete_stack(st);
    delete_banda(b);

    fclose(in);
    fclose(out);
    return 0;
}
// // verifici inserturile si vezi ca s-ar putea sa ai un caracter in plus la
// afisare in fisier si vezi cu \n cum faci la final de fisier
// // UNDO, REDO se fac usor doar adaugi in stiva dupa fiecare move si golesti
// daca
// // dai de write INSERT, ALEA CU MOVE_CHAR de testat

// // OPERATII DE TIP QUERY
// // afisam banda cu tot cu deget

// void show_current(banda *p, FILE *out)
// {
//     //fprintf(out, "%c", p->finger->value);
//     printf("%c", p->finger->value);
// }

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

// // write <c>
// void write(banda *a, char c) { a->finger->value = c; }

// // NETESTATA
// void insert_char(banda *a, char direction, char c, FILE *out) {
//     nod *p = a->finger;
//     printf("caracterul de bagat este %c\n", c);
//     if (direction == 'l') {
//         // nu uita ca head e santinela
//         if (p == a->head->right) {
//             printf("ERROR\n");
//             //fprintf(out, "ERROR\n");
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

//     if (direction == 'r') {
//         if (a->finger->right) {
//             nod *u = a->finger->right;
//             nod *nou = (nod *)malloc(sizeof(nod));
//             p->right = nou;
//             u->left = nou;
//             nou->value = c;
//             nou->left = p;
//             nou->right = u;
//             a->finger = nou;
//         } else {
//             printf("prima greseala\n");
//             nod *nou = (nod *)malloc(sizeof(nod));
//             p->right = nou;
//             nou->value = c;
//             nou->left = p;
//             a->finger = nou;
//         }
//     }
// }

// // OPERATII DE TIP UNDO/REDO
// // void undo(banda *a, stack *undo_st, stack *redo_st) {
// //     push(redo_st, top(undo_st));
// //     if(strstr(top(undo_st), "RIGHT"))
// //         a->finger
// // }

// // eliberam memoria structurilor
// void delete_struct(queue *q, banda *a) {
//     while (!emptyQ(q))
//         popQ(q);  // in caz ca nu vi fi goala desi cred ca va fi mereu
//     // sa scap de coada
//     // printf("coada e coada?:%d\n", emptyQ(q));
//     free(q);
//     // dupa scapi de banda
//     while (a->head != NULL) {
//         pop_banda(a);  // eliberam simultan memoria pentru banda
//         // show(a);
//     }
// }

// for (int i = 1; i <= t; ++i) {
//          fgets(operation, 20, in);
//          printf("operatia este:%s\n", operation);
//          //printf("Lungimea lui operation eeste %ld", strlen(operation));
//          for(int k = 0; k < strlen(operation); ++k)
//              printf("%c", operation[k]);
//          if (strstr(operation, "EXECUTE")) {
//              // scoatem din coada si facem operatia
//              if (!emptyQ(op)) {
//                  o = op->head->value;
//                  popQ(op);
//                  if (strstr(o, "WRITE")) write(&a, o[6]);

//                 if (strstr(o, "MOVE_RIGHT") && !strstr(o, "char")) move(&a,
//                 'r'); if (strstr(o, "MOVE_LEFT") && !strstr(o, "char"))
//                 move(&a, 'l'); if (strstr(o, "MOVE_LEFT_CHAR"))
//                     move_char(&a, 'l', o[15], out);
//                 if (strstr(o, "MOVE_RIGHT_CHAR"))
//                     move_char(&a, 'r', o[16], out);
//                 if (strstr(o, "INSERT_RIGHT"))
//                     insert_char(&a, 'r', o[14], out);
//                 if (strstr(o, "INSERT_LEFT"))
//                     insert_char(&a, 'l', o[13], out);
//             }
//         } else {
//             if (strstr(operation, "UNDO")) {
//                 // printf("face undo\n");
//             } else {
//                 if (strstr(operation, "REDO")) {
//                     // printf("face redo\n");
//                 } else {
//                     if (strstr(operation, "SHOW")) {
//                         show(&a, out);
//                     } else {
//                         if (strstr(operation, "SHOW_CURRENT")) {
//                             show_current(&a, out);
//                         }
//                         // daca nu e execute sau undo/redo sau query
//                         // atunci e
//                         // bagat in coada
//                         else {
//                             addQ(op, operation);  // adaugat   in coada
//                         }
//                     }
//                 }
//             }
//         }
//         show(&a, out);
//         printf("\n");
//     }
//     //stack *st_undo, st_redo;

//     // initializam coada de operatii, banda si stivele
//     op = initQ();
//     init(&a);
//     show(&a, out);
//     //st_undo = initStack();
//     //st_redo = initStack();
//     //  citim numarul de operatii
//     fscanf(in, "%d", &t);
//     printf("ti-a citit t-ul;%d\n", t);
//     // scapi de enter
//     fgets(operation, 20, in);
//

//     delete_struct(op, &a);
//     //fprintf(out, "\n");
