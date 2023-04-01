/* RADU Andrei-Laurentiu - 312CC */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// celulele pentru banda
typedef struct Nod {
    char value;
    struct Nod *left, *right;
} nod;

// celulele pentru coada
typedef struct qCell {
    char *value;
    struct qCell *next;
} qcell;

// banda
typedef struct Banda {
    nod *head, *finger;
} banda;

// coada
typedef struct Queue {
    qcell *front, *tail;
} queue;

// elementele pentru stiva
typedef struct ElemS {
    nod *value;
    struct ElemS *next;
} elemS;

// stiva
typedef struct Stack {
    elemS *head;
} stack;

// alocam memorie pentru qcell(element al cozii)
qcell *initQCell(char *value) {
    qcell *p = (qcell *)malloc(sizeof(qcell));
    // alocam memorie pentru sirul de caractere reprezentat de comanda
    p->value = (char *)malloc(sizeof(char) * (strlen(value) + 1));

    // copiem sirul in celula
    strcpy(p->value, value);
    p->next = NULL;
    return p;
}

// initializam coada
queue *initQ() {
    queue *q = (queue *)malloc(sizeof(queue));
    q->front = q->tail = NULL;
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
    qcell *u = q->front;
    q->front = u->next;

    // eliberam memoria alocata pentru campul value(char *) si qcell in sine
    free(u->value);
    free(u);
}

// adaugam in coada(head, element, element, element, tail)
void addQ(queue *q, char *value) {
    // cream un nou element de adaugat in coada
    qcell *nou = initQCell(value);

    // verificam daca este primul element de adaugat in coada
    if (q->front == NULL) {
        q->front = nou;
        q->tail = nou;
    } else {
        q->tail->next = nou;
        q->tail = nou;
    }
}

// eliberam memoria ocupata de coada
void delete_queue(queue *q) {
    // cat timp coada nu e goala stergem cate un element
    while (!emptyQ(q)) {
        popQ(q);
    }
    // eliberam memoria pentru structura in sine
    free(q);
}

// alocam memorie pentru elemS(element al stivei)
elemS *init_elemS(nod *p) {
    elemS *aux = (elemS *)malloc(sizeof(elemS));
    aux->value = p;
    aux->next = NULL;
    return aux;
}

// initializam stiva
stack *initStack() {
    stack *st = (stack *)malloc(sizeof(stack));
    st->head = NULL;
    return st;
}

// adaugam in stiva
void pushS(stack *st, nod *p) {
    // cream un nou element de adaugat in stiva
    elemS *nou = init_elemS(p);

    // verificam daca este primul element de adaugat in stiva
    if (st->head == NULL) {
        st->head = nou;
        return;
    }

    nou->next = st->head;
    st->head = nou;
    return;
}

// stergem din stiva
void popS(stack *st) {
    elemS *aux = st->head;
    st->head = st->head->next;
    free(aux);
}

// extragem varful stivei
nod *topS(stack *st) { return st->head->value; }

// verificam daca stiva e goala
int emptyS(stack *st) {
    if (st->head == NULL) return 1;
    return 0;
    // 1 = empty
}

// eliberam memoria ocupata de stiva
void delete_stack(stack *st) {
    // cat timp stiva nu e goala stergem cate un element din ea
    while (!emptyS(st)) {
        popS(st);
    }
    free(st);
}

// alocam memorie pentru nod(element al benzii)
nod *initNod(char value) {
    nod *p = (nod *)malloc(sizeof(nod));
    p->value = value;
    p->left = p->right = NULL;
    return p;
}

// cream santinela + prima pozitie
banda *initBanda() {
    // alocam memorie pentru banda
    banda *b = (banda *)malloc(sizeof(banda));
    b->finger = b->head = NULL;

    // punem valoarea santinelei pe '\0' si # pe prima pozitie
    b->head = initNod('\0');
    nod *first = initNod('#');

    // facem cele 2 legaturi(celelalte 2 sunt nule de la initNod)
    b->head->right = first;
    first->left = b->head;

    // punem fingerul pe prima pozitie a benzii
    b->finger = first;
    return b;
}

// verificam daca banda contine doar santinela
int empty_banda(banda *b) {
    // verificam daca e null elementul de dupa santinela
    if (b->head->right == NULL) return 1;
    return 0;
}

// stergem din banda
void pop_banda(banda *b) {
    // santinela
    nod *u = b->head;
    // primul element al benzii
    nod *v = b->head->right;

    // daca e un singur element in banda
    if (v->right == NULL) {
        u->right = NULL;
        // dupa stergere avem dupa santinela NULL
    } else {
        u->right = v->right;
        v->right->left = u;
    }
    free(v);
}

// eliberam memoria ocupata de banda
void delete_banda(banda *b) {
    while (!empty_banda(b)) {
        pop_banda(b);
    }
    // santinela
    free(b->head);
    // structura in sine
    free(b);
}

// OPERATII DE TIP UPDATE
// move_left move_right
void move(banda *b, char direction, stack *undo) {
    if (direction == 'l') {
        // daca nu suntem pe prima pozitie
        if (b->finger != b->head->right) {
            // actualizam stiva de undo cu vechea pozitie a degetului
            pushS(undo, b->finger);
            b->finger = b->finger->left;
        }
        // altfel fingerul ramane pe aceeasi pozitie
    }

    if (direction == 'r') {
        // daca suntem pe ultima pozitie
        if (b->finger->right == NULL) {
            // cream inca un nod cu valoarea '#' pe campul value
            nod *aux = initNod('#');
            b->finger->right = aux;
            aux->left = b->finger;
        }
        // actualizam stiva de undo cu vechea pozitie a degetului
        pushS(undo, b->finger);
        // mutam degetul pe noua pozitie
        b->finger = b->finger->right;
    }
}

// move_left_char <c>, move_right_char <c>
void move_char(banda *b, char direction, char c, FILE *out) {
    nod *aux = b->finger;
    if (direction == 'l') {
        // cat timp nu am ajuns la prima pozitie si nu am gasit caracterul c
        while (aux->left != b->head && aux->value != c) {
            aux = aux->left;
        }

        if (aux->value != c) {
            // daca nu am gasit caracterul c afisam ERROR
            fprintf(out, "ERROR\n");
        } else {
            // mutam degetul pe pozitia caracterului c
            b->finger = aux;
        }
    }

    if (direction == 'r') {
        // cat timp nu am ajuns la ultima pozitie si nu am gasit caracterul c
        while (aux->right != NULL && aux->value != c) {
            aux = aux->right;
        }

        if (aux->value != c) {
            // daca nu am gasit caracterul c cream un alt nod
            nod *nou = initNod('#');
            aux->right = nou;
            nou->left = aux;
            // mutam degetul pe pozitia caracterului c
            b->finger = nou;
        } else {
            // mutam degetul pe pozitia caracterului c
            b->finger = aux;
        }
    }
}

// write <c>
void write(banda *b, char c) { b->finger->value = c; }

// insert_char <c>
void insert_char(banda *b, char direction, char c, FILE *out) {
    nod *v = b->finger;
    if (direction == 'l') {
        if (v == b->head->right) {
            // daca suntem pe prima pozitie afisam ERROR
            fprintf(out, "ERROR\n");
        } else {
            nod *u = b->finger->left;
            // alocam memorie pentru nodul nou
            nod *nou = initNod(c);
            // facem legaturile cu u si v astfel incat sa le avem in ordinea u
            // nou v
            u->right = nou;
            nou->left = u;
            v->left = nou;
            nou->right = v;
            // actualizam pozitia degetului
            b->finger = nou;
        }
    }

    if (direction == 'r') {
        // alocam memorie pentru nodul nou
        nod *nou = initNod(c);
        if (b->finger->right) {
            nod *w = b->finger->right;
            // facem legaturile cu v si w astfel incat sa le avem in ordinea v
            // nou w
            v->right = nou;
            nou->left = v;
            w->left = nou;
            nou->right = w;
        } else {
            v->right = nou;
            nou->left = v;
        }
        // actualizam pozitia degetului
        b->finger = nou;
    }
}

// OPERATII DE TIP QUERY
// show
void show(banda *b, FILE *out) {
    // luam un nod auxiliar care sa primeasca primul element al listei
    nod *aux = b->head->right;
    while (aux != NULL) {
        if (aux == b->finger) {
            // afisam valoarea de la pozitia degetului in formatul |valoare|
            fprintf(out, "|%c|", aux->value);
        } else {
            fprintf(out, "%c", aux->value);
        }
        aux = aux->right;
    }
    fprintf(out, "\n");
}

// show_current
void show_current(banda *b, FILE *out) {
    fprintf(out, "%c\n", b->finger->value);
}

int main() {
    FILE *in = fopen("tema1.in", "r");
    FILE *out = fopen("tema1.out", "w");

    // declaram structurile necesare(2 stive, o coada si banda)
    banda *b;
    stack *undo, *redo;
    queue *q;

    // initializam structurile
    b = initBanda();
    undo = initStack();
    redo = initStack();
    q = initQ();

    // nr = numarul de operatii
    // o = operatia extrasa din coada
    int nr, i;
    char operation[20];
    char *o;

    // citim numarul de operatii
    fscanf(in, "%d", &nr);
    // sa scapam de enterul de dupa nr
    fgets(operation, 20, in);

    for (i = 1; i <= nr; ++i) {
        // citim operatia
        fgets(operation, 20, in);

        // stergem enterul retinut de fgets
        operation[strlen(operation) - 1] = '\0';

        if (strcmp(operation, "EXECUTE") == 0) {
            o = q->front->value;

            /* pentru fiecare operatie extrasa din coada apelam functia
             * corespunzatoare si apoi stergem din coada
             * pentru opeatiile care necesita deplasarem prin banda avem
             * parametrul direction care retine 'l' pentru deplasare la stanga
             * si 'r' pentru deplasare la dreapta*/

            if (strstr(o, "WRITE")) {
                write(b, o[strlen(o) - 1]);
                popQ(q);
                continue;
            }

            if (strstr(o, "MOVE_LEFT_CHAR")) {
                move_char(b, 'l', o[strlen(o) - 1], out);
                popQ(q);
                continue;
            }

            if (strstr(o, "MOVE_RIGHT_CHAR")) {
                move_char(b, 'r', o[strlen(o) - 1], out);
                popQ(q);
                continue;
            }

            if (strstr(o, "MOVE_RIGHT")) {
                move(b, 'r', undo);
                popQ(q);
                continue;
            }

            if (strstr(o, "MOVE_LEFT")) {
                move(b, 'l', undo);
                popQ(q);
                continue;
            }

            if (strstr(o, "INSERT_RIGHT")) {
                insert_char(b, 'r', o[strlen(o) - 1], out);
                popQ(q);
                continue;
            }

            if (strstr(o, "INSERT_LEFT")) {
                insert_char(b, 'l', o[strlen(o) - 1], out);
                popQ(q);
                continue;
            }

            continue;
        }

        // operatia undo permite anularea ultimei operatii
        // de tip move_left sau move_right aplicate
        if (strcmp(operation, "UNDO") == 0) {
            // adaugam in stiva redo ultima pozitie a degetului
            pushS(redo, b->finger);
            // mutam degetul pe pozitia anterioara unei operatii de tip
            // move_right sau move_left
            b->finger = topS(undo);
            // stergem din stiva undo
            popS(undo);
            continue;
        }

        // pentru operatia redo permite refacerea ultimei operatii
        // de tip move_right sau move_left aplicate
        if (strcmp(operation, "REDO") == 0) {
            // adaugam in stiva undo ultima pozitie a degetului
            pushS(undo, b->finger);
            // mutam degetul pe pozitia marcata de operatia
            // move_left sau move_right refacuta
            b->finger = topS(redo);
            // stergem din stiva redo
            popS(redo);
            continue;
        }

        // pentru operatia show afisam banda
        // facand apelul functiei corespunzatoare
        if (strcmp(operation, "SHOW") == 0) {
            show(b, out);
            continue;
        }

        // pentru operatia show_current afisam valoarea de la pozitia degetului
        // facand apelul functiei corespunzatoare
        if (strcmp(operation, "SHOW_CURRENT") == 0) {
            show_current(b, out);
            continue;
        }

        // daca nu este o operatie care se executa direct o adaugam in coada
        addQ(q, operation);
    }

    // stergem structurile eliberand memoria ocupata
    delete_queue(q);
    delete_stack(undo);
    delete_stack(redo);
    delete_banda(b);

    // inchidem fisierele
    fclose(in);
    fclose(out);
    return 0;
}
