# Task 1

## Comanda edit

Citesc numele fisierul de unde urmeaza sa citesc.
Deschid fisierul binar si citesc valorile fileheaderului in variabila fheader.
Citesc infoheaderul in variabila iheader.
Sar peste toti octetii de padding. 
Citesc matricea de pixeli cu ajutorul functiei build_matrix.

## Comanda save

Citesc numele fisierului unde urmeaza sa afisez.
Deschis fisierul binar si scriu in el valorile structurii fheader.
Continui sa scriu in fisier valorile structurii iheader. 
Intre ultimul byte de header si byte-ul matricii de pixeli scriu byte-ul 0.
La scrierea matricei de pixeli sunt atenta sa pun 0 pe bitii de padding. 

## Comanda quit

Eliberez memoria pentru matricea de pixeli.

# Task 2

## Comanda insert
Citesc numele fisierului si pozitia de unde urmeaza sa inserez.
Deschis fiiserul binar si citesc datele noii matrici.
In functia insert schimb valorile matricei salvata la edit cu valorile matricii citite anterior la pozitiile date.

# Task 3

## Comanda set draw_color

Salvez intr-o variabila de tip struct informatiile despre noua culoare

## Comanda set line_width

Salvez in aceeasi variabila de tip struct si dimensiunea creionului.

## Comanda draw line

Citesc cele doua puncte de unde pana unde ar trebui sa trasez linia si apelez functia draw_line.
In functie aflu pe ce axa se afla intervalul maxim si cu ajutorul functiilor line_OX si line_OY determin punctele de pe dreapta
Cu ajutorul functiei drawpoint punctul determinat o sa fie incadrat intr-un patrat de latura data si o sa primeasca o noua valoare.

## Comanda draw rectangle

Apelez functia draw rectangle.
Aflu toate punctele care se afla pe laturile patratului si le colorez cu draw_point.

## Comanda draw triangle

Citesc cele 3 varfuri ale triunghiului.
Apelez functia draw_triangle.
Apelez de 3 ori functa draw_line pentru fiecare pereche de puncte.

# Task 4

Citesc punctul. 
Apelez functia fill.
Pentru fiecare dintre cele 4 directii verific daca punctul se mai afla in matrice si daca indeplineste criteriul din enunt.
Apelez functia pentru noile coodonate si continui tot asa..
