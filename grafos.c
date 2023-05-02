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
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));

    grafo->num_vertices = 0;

    for (int i = 0; i < MAX_VERTICES; i++) {
        grafo->lista_adj[i] = NULL;
    }

    return grafo;
}

void adiciona_vertice(Grafo* grafo, char* vertice) {
    if (grafo->num_vertices < MAX_VERTICES) {
        grafo->lista_adj[grafo->num_vertices] = (No*) malloc(sizeof(No));
        grafo->lista_adj[grafo->num_vertices]->vertice = (char*) malloc(strlen(vertice) + 1);

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

    No* novo = (No*) malloc(sizeof(No));
    novo->vertice = (char*) malloc(strlen(destino) + 1);

    strcpy(novo->vertice, destino);

    novo->prox = grafo->lista_adj[i]->prox;
    grafo->lista_adj[i]->prox = novo;
}

void remove_aresta(Grafo* grafo, char* origem, char* destino) {
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

    No* atual = grafo->lista_adj[i]->prox;
    No* anterior = grafo->lista_adj[i];

    while (atual != NULL) {
        if (strcmp(atual->vertice, destino) == 0) {
            anterior->prox = atual->prox;
            free(atual->vertice);
            free(atual);
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("Erro: aresta não encontrada.\n");
}

Grafo* grafo_transposto(Grafo* grafo) {
    Grafo* transposto = cria_grafo();

    // Preenche o grafo transposto com o vértices do original
    for (int i = 0; i < grafo->num_vertices; i++) {
        adiciona_vertice(transposto, grafo->lista_adj[i]->vertice);
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        No* atual = grafo->lista_adj[i]->prox;

        while (atual != NULL) {
            char* origem = atual->vertice;
            char* destino = grafo->lista_adj[i]->vertice;

            // Verifica se a aresta é um self-loop
            if (strcmp(origem, destino) != 0) {
                if (!existe_aresta(transposto, origem, destino)) {
                    adiciona_aresta(transposto, origem, destino);
                }
            }

            // Avança para a próxima aresta na lista de adjacência
            atual = atual->prox;
        }
    }

    return transposto;
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

void visita_ordem_topologica(Grafo* grafo, int vertice, bool visitados[], int ordem_topologica[], int* indice) {
    visitados[vertice] = true;

    No* atual = grafo->lista_adj[vertice]->prox;

    while (atual != NULL) {
        int vizinho = -1;

        for (int i = 0; i < grafo->num_vertices; i++) {
            if (strcmp(grafo->lista_adj[i]->vertice, atual->vertice) == 0) {
                vizinho = i;
                break;
            }
        }

        if (!visitados[vizinho]) {
            visita_ordem_topologica(grafo, vizinho, visitados, ordem_topologica, indice);
        }

        atual = atual->prox;
    }

    ordem_topologica[*indice] = vertice;
    (*indice)--;
}

int* obtem_ordem_topologica(Grafo* grafo) {
    bool visitados[MAX_VERTICES] = { false };
    int* ordem_topologica = (int*)malloc(sizeof(int) * grafo->num_vertices);
    int indice = grafo->num_vertices - 1;

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (!visitados[i]) {
            visita_ordem_topologica(grafo, i, visitados, ordem_topologica, &indice);
        }
    }

    return ordem_topologica;
}

void visita_componente(Grafo* grafo, int vertice, bool visitados[], int componente[]) {
    visitados[vertice] = true;

    componente[vertice] = 1;

    No* atual = grafo->lista_adj[vertice]->prox;

    while (atual != NULL) {
        int vizinho = -1;

        for (int i = 0; i < grafo->num_vertices; i++) {
            if (strcmp(grafo->lista_adj[i]->vertice, atual->vertice) == 0) {
                vizinho = i;
                break;
            }
        }

        if (!visitados[vizinho]) {
            visita_componente(grafo, vizinho, visitados, componente);
        }

        atual = atual->prox;
    }
}

void encontra_componentes_fortemente_conexas(Grafo* grafo) {
    int* ordem_topologica = obtem_ordem_topologica(grafo);
    bool visitados[MAX_VERTICES] = { false };
    int componente[MAX_VERTICES] = { 0 };
    int num_componentes = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        int vertice = ordem_topologica[i];

        if (!visitados[vertice]) {
            num_componentes++;
            visita_componente(grafo, vertice, visitados, componente);

            printf("Componente fortemente conexa %d: ", num_componentes);
            for (int j = 0; j < grafo->num_vertices; j++) {
                if (componente[j]) {
                    printf("%s ", grafo->lista_adj[j]->vertice);
                }
            }
            printf("\n");
        }
    }

    free(ordem_topologica);
}

int conta_componentes_fortemente_conexas(Grafo* grafo) {
    // Obtém a ordem topológica dos vértices
    int* ordem_topologica = obtem_ordem_topologica(grafo);

    grafo = grafo_transposto(grafo);

    // Percorre a ordem topológica do grafo transposto e conta o número de componentes fortemente conectados
    bool visitados[MAX_VERTICES] = { false };

    int num_componentes = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        int vertice = ordem_topologica[i];

        if (!visitados[vertice]) {
            int componente[MAX_VERTICES] = { 0 };

            visita_componente(grafo, vertice, visitados, componente);

            bool todos_visitados = true;

            for (int j = 0; j < grafo->num_vertices; j++) {
                if (componente[j] == 1 && !visitados[j]) {
                    todos_visitados = false;
                    break;
                }
            }

            if (todos_visitados) {
                num_componentes++;
            }
        }
    }

    return num_componentes;
}

bool eh_fortemente_conectado(Grafo* grafo) {
    if (conta_componentes_fortemente_conexas(grafo) == 1) {
      return true;
    }

    return false;
}

void imprime_ordem_topologica(Grafo* grafo) {
    int* ordem_topologica = obtem_ordem_topologica(grafo);

    for (int i = 0; i < grafo->num_vertices; i++) {
        printf("%s ", grafo->lista_adj[ordem_topologica[i]]->vertice);
    }

    printf("\n");

    free(ordem_topologica);
}

void concatena_componente(Grafo* grafo, int vertice, bool visitados[], char* componente) {
    visitados[vertice] = true;

    strcat(componente, grafo->lista_adj[vertice]->vertice);
    strcat(componente, " ");

    No* atual = grafo->lista_adj[vertice]->prox;

    while (atual != NULL) {
        int vizinho = -1;

        for (int i = 0; i < grafo->num_vertices; i++) {
            if (strcmp(grafo->lista_adj[i]->vertice, atual->vertice) == 0) {
                vizinho = i;
                break;
            }
        }

        if (!visitados[vizinho]) {
            concatena_componente(grafo, vizinho, visitados, componente);
        }

        atual = atual->prox;
    }
}

void concatena_componentes_fortemente_conexas(Grafo* grafo) {
    int* ordem_topologica = obtem_ordem_topologica(grafo);
    bool visitados[MAX_VERTICES] = { false };

    for (int i = 0; i < grafo->num_vertices; i++) {
        int vertice = ordem_topologica[i];

        if (!visitados[vertice]) {
            char componente[MAX_VERTICES] = { 0 };
            concatena_componente(grafo, vertice, visitados, componente);
        }
    }
}

void destroi_grafo(Grafo* grafo) {
  int i;

  for (i = 0; i < grafo->num_vertices; i++) {
      No* atual = grafo->lista_adj[i]->prox;

      while (atual != NULL) {
          No* proximo = atual->prox;
          free(atual->vertice);
          free(atual);
          atual = proximo;
      }

      free(grafo->lista_adj[i]->vertice);
      free(grafo->lista_adj[i]);
  }

  free(grafo);
}


int main() {
  Grafo* grafo = cria_grafo();

  adiciona_vertice(grafo, "abe");
  adiciona_vertice(grafo, "cd");
  adiciona_vertice(grafo, "fg");
  adiciona_vertice(grafo, "h");

  adiciona_aresta(grafo, "abe", "cd");
  adiciona_aresta(grafo, "abe", "fg");
  adiciona_aresta(grafo, "cd", "fg");
  adiciona_aresta(grafo, "cd", "h");
  adiciona_aresta(grafo, "fg", "h");

  if (eh_fortemente_conectado(grafo))
    printf("sim\n");

  else
    printf("nao\n");

  printf("%d\n", conta_componentes_fortemente_conexas(grafo));

  concatena_componentes_fortemente_conexas(grafo);

  imprime_ordem_topologica(grafo);

  imprime_grafo(grafo);

  return 0;
}
