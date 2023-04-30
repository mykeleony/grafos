#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    } else {
        printf("Erro: número máximo de vértices atingido.\n");
    }
}

void adiciona_aresta(Grafo* grafo, char* origem, char* destino) {
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

void imprime_grafo(Grafo* grafo) {
    printf("Grafo:\n");
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
  Grafo* grafo = cria_grafo();

  adiciona_vertice(grafo, "A");
  adiciona_vertice(grafo, "B");
  adiciona_vertice(grafo, "C");
  adiciona_vertice(grafo, "D");
  adiciona_vertice(grafo, "E");

  adiciona_aresta(grafo, "A", "B");
  adiciona_aresta(grafo, "B", "C");
  adiciona_aresta(grafo, "B", "A");
  adiciona_aresta(grafo, "B", "A");
  adiciona_aresta(grafo, "C", "D");
  adiciona_aresta(grafo, "D", "A");

  imprime_grafo(grafo);

  return 0;
}
