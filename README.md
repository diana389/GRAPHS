# GRAPHS

Datele introduse de la tastatura: nr de noduri, nr de muchii, nr de depozite, muchiile (cu care s-a format matricea de adiacenta), depozitele, nr cerintelor, apoi cerintele, intrand pe cazul corespunzator pentru a le rezolva.

* ##  TASK 1:
Am calculat costul minim al drumului dintre un depozit si un magazin, apoi dintre magazin si depozit si am retinut traseul parcurs, folosind algoritmul dijkstra.

###  `DIJKSTRA`:
Elementele vectorului de distante iau valoarea maxima, pentru ca urmeaza sa se calculeze un minim. Distanta de la sursa la ea insasi este 0. Se alege nodul spre 
care este distanta minima, cu ajutorul functiei `minDistance`, si se marcheaza ca vizitat. Se compara distantele pentru a actualiza vectorul de distante, retinand 
si ordinea nodurilor (vectorul ***"parent"***). Procesul se repeta pana cand destinatia este marcata ca vizitata. Se apeleaza functia `make_tr`, care pune recursiv in
vectorul ***"semi_traseu"***, traseul de la sursa la destinatie, in ordine inversa. Apoi, este pus in ordinea corecta in vectorul ***"traseu"***, ce retine ambele jumatati de drum.  

* ##  TASK 2:
Am afisat componentele tare conexe ale grafului, in ordine crescatoare, folosind functia `strongly_connected`.

###  `STRONGLY_CONNECTED`:
Am construit o noua matrice de adiacenta care, initial, sa aiba 1 pe toate pozitiile in care exista muchie si 0, in rest. Depozitele se ignora in aceasta cerinta, deci se anuleaza toate arcele acestora. Apoi, pentru fiecare 2 noduri, am apelat functia `dijkstra`, iar daca distanta este diferita de **INT_MAX** (adica a fost gasit un drum intre ele), elementul corespunzator din matrice devine 1. Astfel, avem 1 pe toate pozitiile in care se poate ajunge dintr-un nod in celalalt. In vectorul de noduri vizitate, depozitele se marcheaza cu 1 pentru a fi ignorate si se apeleaza functia `print_components`, care afiseaza nr de componente tari conexe si componentele.

###  `PRINT_COMPONENTS`:
Functia este recursiva si se apeleaza la jumatate, facand intai prima parte, iar apoi a doua, in ordine inversa. Se parcurge fiecare nod, dinspre ultimul spre 
primul, iar daca nodul nu este vizitat, numarul de componente tare conexe creste si este apoi marcat ca vizitat. Toate nodurile la care se poate ajunge si din care
se poate ajunge la nodul curent sunt marcate ca vizitate, pentru ca fac parte din acceasi componenta conexa. Cand trece de primul nod, se afiseaza numarul de
componente conexe si variabilele isi reiau valorile initiale, deoarece procesul se repeta in a doua jumatate, de la primul nod spre ultimul, afisandu-se si 
componentele.

* ##  TASK 3:
Am citit si am retinut nodurile dintr-o zona in vectorul ***"tr"***, urmand ca pentru fiecare depozit acesta sa fie adaugat la sfarsit si sa se apeleze functia `area`, ce returneaza distanta minima a traseului prin care se parcurg toate nodurile din zona. Distanta minima dintre acestea este afisata.

###  `AREA`:
Am implementat o lista dublu inlantuita care sa contina traseul parcurs pana in momentul respectiv, avand pentru fiecare nod si un vector de frecventa pentru
nodurile ce il mai pot urma. Am construit o matrice care sa contina doar arcele dintre nodurile din zona respectiva. <br>
Nodul de la care se porneste este depozitul. Apoi, se cauta nodul spre care costul este minim, care este inca posibil sa fie plasat urmatorul si care, daca nu este 
sursa, a fost vizitat de maxim o data, pentru a nu se ajunge in cicluri infinte. <br>
In cazul in care nu se mai gaseste un astfel de nod, se revine la nodul anterior, micsorand si distanta curenta si marcand nodul curent pentru a nu mai putea fi 
vizitat in aceasta ordine. In cazul in care nodul a fost gasit, se adauga in lista, distanta curenta creste si se verifica daca traseul contine toate nodurile din zona (si daca depozitul se afla pe ultima pozitie) prin functia `is_complete`. In caz afirmativ, se compara distanta cu cea minima si se revine la un nod anterior. Acest proces de repeta pana cand se scot toate nodurile din lista.




