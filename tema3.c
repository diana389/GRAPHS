#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

// functie pentru distanta minima
int minDistance(float *dist, bool *viz, float **graph, int V)
{
    int j, min = INT_MAX, index = V;

    for (j = 0; j < V; j++)
        if (!viz[j] && dist[j] < min)
        {
            min = dist[j];
            index = j;
        }

    return index;
}

// functie pentru formarea traseului pascurs
void make_tr(int *parent, int j, int *semi_traseu, int *k)
{
    semi_traseu[(*k)++] = j;
    if (parent[j] == -1)
        return;

    make_tr(parent, parent[j], semi_traseu, k);
}

// algoritmul dijkstra cu retinerea traseului parcurs
void dijkstra(float **graph, int src, int dest, float *dist, int *traseu, int *k, int V)
{
    int i, j, u, k1 = 0;
    bool viz[1000] = {0};
    int parent[1000] = {-1}, semi_traseu[1000];

    for (i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (i = 0; i < V && !viz[dest]; i++)
    {
        // Se alege distanta minima dintre nodurile nevizitate
        u = minDistance(dist, viz, graph, V);

        // Nodul se marcheaza ca vizitat.
        viz[u] = 1;

        for (j = 0; j < V; j++)
            if (u < V && !viz[j] && graph[u][j] && dist[u] != INT_MAX && dist[u] + graph[u][j] < dist[j])
            {
                dist[j] = dist[u] + graph[u][j];
                parent[j] = u; // Se formeaza traseul.
            }
    }

    // Nodurile se pun in vectorul "semi_traseu" in ordine inversa.
    make_tr(parent, dest, semi_traseu, &k1);

    // Se inverseaza ordinea.
    for (i = 0; i < k1; i++)
        traseu[(*k) + i] = semi_traseu[k1 - i - 1];

    *k += k1;
}

void print_components(float **mat, int V, bool *viz, int i, int *nr, int *dep, int nr_dep)
{
    int j;
    if (i == -1)
    {
        printf("%d", *nr);

        // Se reia starea initiala.
        for (j = 0; j < V; j++)
            viz[j] = 0;
        for (j = 0; j < nr_dep; j++)
            viz[dep[j]] = 1;

        return;
    }

    if (!viz[i])
    {
        viz[i] = 1;
        (*nr)++;
    }

    // Se marcheaza toate nodurile nevizitate care au legatura cu i,
    // pentru a le grupa in acceasi componenta tare conexa.
    for (j = 0; j < V; j++)
        if (mat[i][j] && mat[j][i] && !viz[j])
            viz[j] = 1;

    // Functia este apelata pentru fiecare nod.
    print_components(mat, V, viz, i - 1, nr, dep, nr_dep);

    // Se afiseaza componentele tare conexe.
    if (!viz[i])
    {
        printf("\n%d", i);
        viz[i] = 1;
    }

    // Se marcheaza toate nodurile nevizitate care au legatura cu i,
    // pentru a le grupa in acceasi componenta tare conexa.
    for (j = 0; j < V; j++)
        if (mat[i][j] && mat[j][i] && !viz[j])
        {
            printf(" %d", j);
            viz[j] = 1;
        }
}

void strongly_connected(float **graph, int V, int *dep, int nr_dep)
{
    int i, j;
    float **mat = (float **)malloc(V * sizeof(float *));
    for (i = 0; i < V; i++)
        mat[i] = (float *)calloc(V, sizeof(float));

    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            if (graph[i][j])
                mat[i][j] = 1;

    // Se ignora depozitele, deci se construieste o noua matrice fara leaturile cu acestea.
    for (i = 0; i < nr_dep; i++)
        for (j = 0; j < V; j++)
        {
            mat[dep[i]][j] = 0;
            mat[j][dep[i]] = 0;
        }

    // Am apelat dijkstra intre fiecare 2 noduri pentru a afla daca exista o cale intre ele.
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
        {
            int traseu[1000], k = 0;
            float dist[1000] = {0};

            dijkstra(mat, i, j, dist, traseu, &k, V);

            if (dist[j] != INT_MAX)
                mat[i][j] = 1;
        }

    bool viz[100] = {0};

    // Depozitele se marcheaza cu 1 pentru a fi ignorate.
    for (i = 0; i < nr_dep; i++)
        viz[dep[i]] = 1;

    int nr = 0;
    print_components(mat, V, viz, V - 1, &nr, dep, nr_dep);

    for (i = 0; i < V; i++)
        free(mat[i]);
    free(mat);
}

typedef struct Node
{
    int id;
    int next_pos[100]; // urmatoarele posibile noduri
    struct Node *next, *prev;
} Node;

// Functie pentru adaugarea unui nod la finalul cozii.
Node *add_node(Node **traseu, int u, int V)
{
    Node *next_node = (Node *)malloc(sizeof(Node));
    next_node->id = u;

    int i;
    for (i = 0; i < V; i++)
        next_node->next_pos[i] = 0;

    if (*traseu == NULL)
    {
        (*traseu) = next_node;
        (*traseu)->prev = NULL;
        (*traseu)->next = NULL;
        return next_node;
    }

    Node *p = *traseu;
    while (p->next != NULL)
        p = p->next;

    next_node->next = NULL;
    next_node->prev = p;
    p->next = next_node;
    return next_node;
}

// functie pentru stergerea unui nod din coada
Node *pop_node(Node **last)
{
    if ((*last)->prev != NULL)
    {
        Node *p = (*last)->prev;
        free(*last);
        p->next = NULL;
        return p;
    }
    else
        return NULL;
}

// functie care verifica daca traseul cuprinde toate nodurile
int is_complete(int *tr, int Vtr, Node *p, int *viz)
{
    int i;
    if (tr[Vtr - 1] != p->id)
        return 0;

    for (i = 0; i < Vtr; i++)
        if (!viz[tr[i]])
            return 0;

    return 1;
}

// functie pentru eliberarea memoriei
void free_list(Node **traseu)
{
    Node *p = *traseu;
    Node *next;

    while (p != NULL)
    {
        next = p->next;
        free(p);
        p = next;
    }
    *traseu = NULL;
}

float area(float **graph, int V, int src, int *tr, int Vtr)
{
    int i, j;
    Node *traseu = NULL; // primul nod in lista
    Node *p = traseu;    // nodul curent

    p = add_node(&traseu, src, V); // Se adauga sursa in lista.

    float **mat = (float **)malloc(V * sizeof(float *));
    for (i = 0; i < V; i++)
        mat[i] = (float *)calloc(V, sizeof(float));

    // Se construieste o matrice doar cu nodurile prin care trebuie sa se treaca.
    for (i = 0; i < Vtr; i++)
        for (j = 0; j < Vtr; j++)
            mat[tr[i]][tr[j]] = graph[tr[i]][tr[j]];

    int u = src, stop = 0, next;
    int viz[1000] = {0};
    float dist_min = INT_MAX, cur_dist = 0, min;

    while (!stop)
    {
        min = INT_MAX;
        int gasit = 0;

        for (j = 0; j < V; j++)
        {
            // Se alege valoarea minima dintre nodurile inca posibile.
            if (mat[u][j] > 0 && mat[u][j] < min && (traseu == NULL || !p->next_pos[j]))
                if ((viz[j] < 2 && j != tr[Vtr - 1]) || (viz[j] < 5 && j == tr[Vtr - 1]))
                {
                    min = mat[u][j];
                    gasit = 1;
                    next = j;
                }
        }

        if (!gasit) // daca nu mai exista noduri posibile
        {
            if (p)
            {
                if (p->prev != NULL)
                {
                    cur_dist -= mat[p->prev->id][u]; // Se scade ultima muchie ("se da cu un pas inapoi").
                    p->prev->next_pos[p->id] = -1;   // Nodul prin care am trecut nu mai este o varianta, pentru a nu ajunge intr-un ciclu infinit.

                    viz[p->id]--; // Nr de aparitii ale nodului in traseu scade.
                }
                p = pop_node(&p);
            }

            if (p == NULL)
            {
                stop = 1;
                break;
            }

            u = p->id;
        }
        else // daca exista noduri posibile
        {
            u = next;
            viz[u]++;
            cur_dist = cur_dist + min; // Costul se adauga la distanta curenta.

            p = add_node(&traseu, u, V);

            if (is_complete(tr, Vtr, p, viz)) // daca traseul este complet
            {
                if (cur_dist < dist_min)
                    dist_min = cur_dist;

                cur_dist -= mat[p->prev->id][p->id]; // Se scade ultima muchie ("se da cu un pas inapoi").
                p->prev->next_pos[p->id] = -1;       // Nodul prin care am trecut nu mai este o varianta, pentru a nu ajunge intr-un ciclu infinit.

                viz[p->id]--;     // Nr de aparitii ale nodului in traseu scade.
                p = pop_node(&p); // Se elimina din coada.

                u = p->id;
            }
        }
    }

    for (i = 0; i < V; i++)
        free(mat[i]);
    free(mat);

    free_list(&traseu);
    return dist_min;
}

int main()
{
    int V, i, j, nr_muchii, nr_dep, m, dep[10], nr_tasks, dep_src, nr_mag = 1, mag;
    float cost;
    char task[3];

    scanf("%d%d%d", &V, &nr_muchii, &nr_dep);

    float **graph = (float **)malloc(V * sizeof(float *));
    for (i = 0; i < V; i++)
        graph[i] = (float *)malloc(V * sizeof(float));

    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
            graph[i][j] = 0;
    }

    for (m = 0; m < nr_muchii; m++)
    {
        scanf("%d%d%f", &i, &j, &cost);
        graph[i][j] = cost;
    }

    for (i = 0; i < nr_dep; i++)
        scanf("%d", &dep[i]);

    scanf("%d", &nr_tasks);

    for (i = 0; i < nr_tasks; i++)
    {
        scanf("%s", task);

        if (strcmp("e1", task) == 0) // task 1
        {
            scanf("%d", &dep_src);
            scanf("%d", &nr_mag);

            float total_cost = 0;

            for (j = 0; j < nr_mag; j++)
            {
                scanf("%d", &mag);
                printf("%d\n", mag);

                int traseu[1000], start_index = 0, x;
                float dist[1000] = {0};
                float dist_dus = 0, dist_intors = 0;

                // drumul de la depozit la magazin
                dijkstra(graph, dep_src, mag, dist, traseu, &start_index, V);
                dist_dus = dist[mag];

                start_index--;

                // drumul de la magazin la depozit
                dijkstra(graph, mag, dep_src, dist, traseu, &start_index, V);
                dist_intors = dist[dep_src];

                printf("%.1lf ", dist_dus);
                printf("%.1lf\n", dist_intors);
                total_cost += dist_dus + dist_intors;

                for (x = 0; x < start_index - 1; x++)
                    printf("%d ", traseu[x]);
                printf("%d\n", traseu[x]);
            }

            printf("%.1lf", total_cost);
        }

        if (strcmp("e2", task) == 0) // task 2
            strongly_connected(graph, V, dep, nr_dep);

        if (strcmp("e3", task) == 0) // task 3
        {
            int nr_zone, Vtr, x;
            float dist, dist_min = INT_MAX;
            int tr[100]; // vector cu nodurile dintr-o zona

            scanf("%d", &nr_zone);

            for (j = 0; j < nr_zone; j++)
            {
                scanf("%d", &Vtr);

                for (x = 0; x < Vtr; x++)
                    scanf("%d", &tr[x]);
                Vtr++;

                // Se calculeaza distanta pt fecare departament si se afla minimul
                for (x = 0; x < nr_dep; x++)
                {
                    tr[Vtr - 1] = dep[x];

                    dist = area(graph, V, dep[x], tr, Vtr);

                    if (dist < dist_min)
                        dist_min = dist;
                }

                printf("%.1lf", dist_min);
                if (j < nr_zone - 1)
                    printf("\n");
            }
        }

        if (i < nr_tasks - 1)
            printf("\n");
    }

    for (i = 0; i < V; i++)
        free(graph[i]);
    free(graph);

    return 0;
}