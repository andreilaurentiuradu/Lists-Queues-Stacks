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

// alocare celula pentru coada
cell *initCell(char *value) {
    cell *p = (cell *)malloc(sizeof(cell));
    //printf("value: %s\n", value);

    // alocam memorie pentru sirul de caractere reprezentat de comanda
    p->value = (char *)malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(p->value, value);
    //printf("s-a pus in p->value:%s\n", p->value);
    p->next = NULL;
    return p;
}
// initializam coada
queue *initQ() {
    queue *q = (queue *)malloc(sizeof(queue));
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
}

// adaugare in coada(head, element, element, element, tail)
void addQ(queue *q, char *value) {
    cell *nou = initCell(value);
    //printf("nou->value:%s\n", nou->value);
    if (!q->length) {
        q->head = nou;
        q->tail = nou;
        //printf("s-a creat primul nod\n");
        // strcpy(q->head->value, op);
        // strcpy(q->tail->value, op);
    } else {
        // strcpy(nou->value, op);
        q->tail->next = nou;
        q->tail = nou;
        // q->tail = NULL;
    }
    (q->length)++;
}

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

// OPERATII DE TIP UPDATE(toate operatiile merg, STERGE CONTORUL)
// move_left move_right
char contor = '0';
void move(banda *a, char direction) {
    if (direction == 'l') {
        if (a->finger !=
            a->head
                ->right)  // daca nu suntem pe prima pozitie (head e santinela)
            a->finger = a->finger->left;
    }

    if (direction == 'r') {
        if (a->finger->right == NULL) {
            nod *aux = (nod *)malloc(sizeof(nod));
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
void write(banda *a, char c) { a->finger->value = c; }

int main() {
    // banda
    banda a;
    // coada de operatii
    queue *op;
    // numarul de operatii
    int t;
    // numele operatiilor
    char operation[20];

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

        //printf("operatie:%s\n", operation);
        if (strcmp(operation, "EXECUTE") == 0) {
            // scoatem din coada si facem operatia
            char *o = (char *)malloc(sizeof(char) * (strlen(op->head->value) + 1));
            o = op->head->value;
            popQ(op);
            if(strstr(o, "WRITE"))
                write(&a, o[strlen(o)-1]);

            if(strcmp(o, "MOVE_RIGHT") == 0)
                move(&a, 'r');
            if(strcmp(o, "MOVE_LEFT") == 0)
                move(&a, 'l');
            //if(strcmp(o, "MOVE"))
        } else {
            if (strcmp(operation, "UNDO") == 0) {
            } else {
                if (strcmp(operation, "REDO") == 0) {
                } else {
                    if (strcmp(operation, "SHOW") == 0) {
                        show(&a);
                    } else {
                        if (strcmp(operation, "SHOW_CURRENT") == 0) {
                            show_current(&a);
                        }
                        // daca nu e execute sau undo/redo sau query atunci e
                        // bagat in coada
                        else
                            addQ(op, operation);  // adaugam in coada
                    }
                }
            }
        }

        printf("dupa operatie: ");
        show(&a);
    }

    return 0;
}




// while(!emptyQ(q))
//     popQ(q);
// free(q); // sa scap de coada
// dupa scapi de banda
