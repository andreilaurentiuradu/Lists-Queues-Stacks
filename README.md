# Tema-1-SDA
liste, cozi si stive
312CC Radu Andrei-Laurentiu Tema1 SDA

Problema consta in crearea unei "benzi" pentru stocarea datelor
pe care se executa operatii de tip UPDATE/QUERY/UNDO/REDO/EXECUTE

Banda este creata ca o structura ce contine:
-o lista dublu inlantuita cu santinela
-adresa "degetului" pus de David pe banda

Pentru banda am implementat urmatoarele functii:
-initNod pentru initilizarea elementelor benzii
-initBanda pentru initializarea benzii
-empty_Banda pentru a verifica daca banda este goala
-pop_banda pentru stergerea unui element din banda si eliberarea memoriei
-delete_banda pentru eliberarea memoriei ocupate de banda si de elementele ei
-functiile pentru efectuarea operatiilor 

OPERATII DE TIP UPDATE
Pentru opeatiile de tip UPDATE cream o structura de tip coada care va retine
o lista simplu inlantuita, fiind executate in momentul in care apare 
operatia EXECUTE.
Daca s-a citit o operatie de tip UPDATE se adauga in coada

Pentru coada am implementat urmatoarele functii:
-initQCell pentru initializarea elementelor cozii
-initQ pentru initializarea cozii
-emptyQ pentru a verifica daca coada este goala
-popQ pentru stergerea unui element din coada si eliberarea memoriei 
-addQ pentru adaugarea unui element in coada
-delete_queue pentru stergerea elementelor ramase in coada
(in cazul in care nu se dau destule operatii de tip EXECUTE)
si eliberarea memoriei ocupate de structura in sine

Pentru MOVE_RIGHT si MOVE_LEFT am implementat functia move
care are ca parametrii banda, directia de deplasare si stiva de undo

Pentru MOVE_CHAR_LEFT si MOVE_CHAR_RIGHT am implementat functia move_char
care are ca parametrii banda, directia de deplasare si fisierul in care afisam

Pentru INSERT_LEFT si INSERT_RIGHT am implementat functia insert_char
care are ca parametrii banda, directia de deplasare si fisierul in care afisam


OPERATII DE TIP QUERY

Pentru operatiile SHOW si SHOW_CURRENT am implementat functiile 
show(afisarea benzii), respectiv show_current(afisarea elementului 
de la adresa degetului)

OPERATII DE TIP UNDO/REDO
Pentru operatiile de tip UNDO/REDO am creat cate o structura de tip stiva care
va retine o lista simplu inlantuita

Pentru stiva am implementat urmatoarele functii:
-init_elemS pentru initializarea unui element din stiva
-initStack pentru initializarea stivei
-pushS pentru adaugarea unui element in stiva
-popS pentru stergerea unui element din stiva si eliberarea memoriei
-topS pentru afisarea elementului din varful stivei
-emptyS pentru a verifica daca stiva e goala
-delete_stack stergem elementele din stiva si eliberam memoria

In momentul in care facem o operatie de tip MOVE_LEFT sau MOVE_RIGHT
adaugam in stiva de undo pozitia anterioara a degetului.
Atunci cand executam operatia UNDO, extragem pointerul retinut in 
varful stivei pentru UNDO, adaugam pointerul la pozitia curenta a degetului 
in varful stivei pentru REDO, dupa care modificam pozitia degetului astfel 
incat sa indice catre pointerul extras din stiva pentru UNDO.
Atunci cand executam operatia REDO, extragem pointerul retinut in varful stivei 
pentru REDO, adaugam pointerul la pozitia curenta a degetului in varful stivei
pentru UNDO, dupa care modificam pozitia degetului astfel incat sa indice catre 
pointerul extras din stiva pentru REDO.
Dupa o operatie de WRITE stivele se golesc, pentru ca se garanteaza ca nu se 
vor da operatii de UNDO/REDO peste cele de WRITE


Punctaj teste:
Total 100.0/100
Valgrind 20/20
