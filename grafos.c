#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 1000

typedef struct _no {
    char* vertice;
    struct _no* prox;
} No;

typedef struct _grafo {
    No* lista_adj[MAX_VERTICES];
    int num_vertices;
} Grafo;

Grafo* cria_grafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));

    grafo->num_vertices = 0;

    for (int i = 0; i < MAX_VERTICES; i++) {
        grafo->lista_adj[i] = NULL;
    }

    return grafo;
}

void adiciona_vertice(Grafo* grafo, char* vertice) {
    if (grafo->num_vertices < MAX_VERTICES) {
        grafo->lista_adj[grafo->num_vertices] = (No*)malloc(sizeof(No));
        grafo->lista_adj[grafo->num_vertices]->vertice = (char*)malloc(strlen(vertice) + 1);

        strcpy(grafo->lista_adj[grafo->num_vertices]->vertice, vertice);

        grafo->lista_adj[grafo->num_vertices]->prox = NULL;
        grafo->num_vertices++;
    }

    else {
        printf("Erro: número máximo de vértices atingido.\n");
    }
}

bool existe_aresta(Grafo* grafo, char* origem, char* destino) {
    int i, j;

    for (i = 0; i < grafo->num_vertices; i++) {
        if (strcmp(grafo->lista_adj[i]->vertice, origem) == 0) {
            break;
        }
    }

    if (i == grafo->num_vertices) {
        return false;
    }

    No* atual = grafo->lista_adj[i]->prox;

    while (atual != NULL) {
        if (strcmp(atual->vertice, destino) == 0) {
            return true;
        }
        atual = atual->prox;
    }

    return false;
}

void adiciona_aresta(Grafo* grafo, char* origem, char* destino) {
    if (existe_aresta(grafo, origem, destino)) {
        return;
    }

    int i;

    for (i = 0; i < grafo->num_vertices; i++) {
        if (strcmp(grafo->lista_adj[i]->vertice, origem) == 0) {
            break;
        }
    }

    if (i == grafo->num_vertices) {
        printf("Erro: vértice de origem não encontrado.\n");
        return;
    }

    No* novo = (No*)malloc(sizeof(No));
    novo->vertice = (char*)malloc(strlen(destino) + 1);

    strcpy(novo->vertice, destino);

    novo->prox = grafo->lista_adj[i]->prox;
    grafo->lista_adj[i]->prox = novo;
}

// Erdos-Renyi
void preenche_grafo_aleatoriamente(Grafo* grafo, int num_vertices, int num_arestas) {
    char* vertices[num_vertices];

    int i;

    for (i = 0; i < num_vertices; i++) {
        char* nome_vertice = (char*) malloc(2 * sizeof(char));

        nome_vertice[0] = 'A' + i; // Nomeia vertice
        nome_vertice[1] = '\0';

        vertices[i] = nome_vertice;

        adiciona_vertice(grafo, nome_vertice);
    }

    int num_arestas_criadas = 0;

    while (num_arestas_criadas < num_arestas) {
        // Escolhe dois vértices aleatoriamente
        int indice_origem = rand() % num_vertices;
        int indice_destino = rand() % num_vertices;

        if (indice_origem != indice_destino) {  // Impede self-loops
            char* origem = vertices[indice_origem];
            char* destino = vertices[indice_destino];
            adiciona_aresta(grafo, origem, destino);
            num_arestas_criadas++;
        }
    }
}

static void busca_em_profundidade_visit(Grafo* G, int u, bool* visitado, int* tempo, int* d, int* f) {
    visitado[u] = true;
    tempo[0] += 1;
    d[u] = tempo[0];

    No* v = G->lista_adj[u];

    while (v != NULL) {
        int index = atoi(v->vertice);
        if (!visitado[index]) {
            busca_em_profundidade_visit(G, index, visitado, tempo, d, f);
        }

        v = v->prox;
    }

    tempo[0] += 1;
    f[u] = tempo[0];
}

static Grafo* transpoe_grafo(Grafo* G) {
    Grafo* GT = cria_grafo();

    for (int i = 0; i < G->num_vertices; i++) {
        No* v = G->lista_adj[i];

        while (v != NULL) {
            adiciona_aresta(GT, v->vertice, G->lista_adj[i]->vertice);
            v = v->prox;
        }
    }

    return GT;
}

static bool busca_em_profundidade(Grafo* G, int n) {
    bool* visitado = (bool*)calloc(MAX_VERTICES, sizeof(bool));
    int* d = (int*)calloc(MAX_VERTICES, sizeof(int));
    int* f = (int*)calloc(MAX_VERTICES, sizeof(int));
    int tempo = 0;

    busca_em_profundidade_visit(G, n, visitado, &tempo, d, f);

    for (int i = 0; i < G->num_vertices; i++) {
        if (!visitado[i]) {
            return false;
        }
    }

    Grafo* GT = transpoe_grafo(G);
    memset(visitado, false, MAX_VERTICES * sizeof(bool));
    busca_em_profundidade_visit(GT, n, visitado, &tempo, d, f);

    for (int i = 0; i < GT->num_vertices; i++) {
        if (!visitado[i]) {
            return false;
        }
    }

    return true;
}

bool grafo_eh_fortemente_conectado(Grafo* G) {
    for (int i = 0; i < G->num_vertices; i++) {
        if (!busca_em_profundidade(G, i)) {
            return false;
        }
    }

    return true;
}

void imprime_grafo(Grafo* grafo) {
    for (int i = 0; i < grafo->num_vertices; i++) {
        printf("%s: ", grafo->lista_adj[i]->vertice);

        No* atual = grafo->lista_adj[i]->prox;

        while (atual != NULL) {
            printf("%s; ", atual->vertice);
            atual = atual->prox;
        }

        printf("\n");
    }
}

int main() {
  return 0;
}
