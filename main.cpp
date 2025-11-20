#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // pro toupper()

#define MAX_VERTICES 10
#define INFINITO 9999


void imprimirGrafo(int matriz[MAX_VERTICES][MAX_VERTICES], int num_vertices);
void limpaTela();

void dijkstra(int grafo[MAX_VERTICES][MAX_VERTICES], int num_vertices, int inicio, int fim);

int main() {
    int num_vertices, peso;
    int matriz[MAX_VERTICES][MAX_VERTICES];
    char letraOrigem, letraDestino;
    char start_char, end_char;

    // inicia matriz
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            matriz[i][j] = 0;
        }
    }

    // -- pede o tamnho da matriz
    do {
        printf("Qual o tamanho da sua matriz? (MAX: 10): ");
        scanf("%d", &num_vertices);
        fflush(stdin);

        if (num_vertices >= 2 && num_vertices <= MAX_VERTICES) {
            break;
        }

        printf("Entrada errada, insira um numero entre 2 e 10!!\n");
    } while (num_vertices < 2 || num_vertices > MAX_VERTICES);

    limpaTela();
    printf("\nSeus vertices sao: ");
    for (int i = 0; i < num_vertices; i++) {
        printf("%c ", 'A' + i);
    }

    printf("\n\n");


    // agora começa a popular o grafo e liga;çoes
    printf("--- Criando Grafo ---\n");
    printf("Digite as conexoes no formato: ORIGEM DESTINO PESO\nExemplo: A B 10 (conecta A e B com custo 10)\n");
    printf("Digite 'X X 0' para encerrar.\n----------------\n\n");

    // for pra preencher matriz adjacente
    while (1) {
        // pede a conexao
        printf("Nova conexao: ");
        scanf(" %c %c %d", &letraOrigem, &letraDestino, &peso);

        // upper pra quando digitar 'a'
        letraOrigem = toupper(letraOrigem);
        letraDestino = toupper(letraDestino);

        if (letraOrigem == 'X' || letraDestino == 'X') {
            break; // mata o loop se for X X
        }

        // converte de letra pra numero como na tabela ASCII
        int origemInNumero = letraOrigem - 65;
        int destinoInNumero = letraDestino - 65;

        // as letra "existem"?
        if (origemInNumero >= 0
            && origemInNumero < num_vertices
            && destinoInNumero >= 0
            && destinoInNumero < num_vertices
            && peso > 0) {
            // coloca matriz
            matriz[origemInNumero][destinoInNumero] = peso;
            matriz[destinoInNumero][origemInNumero] = peso;

            printf("    >> Sucesso! conexao: %c <--> %c (Peso %d) == OK\n", letraOrigem, letraDestino, peso);
        } else {
            printf("    Erro! Vertices invalidos ou peso negativo. Tente novamente.\n");
            continue;
        }
    }


    limpaTela();
    printf("\nCadastro finalizado!\n");
    imprimirGrafo(matriz, num_vertices);

    printf("\nPara qual posicao voce deseja ir? Digite a letra correspondende ao 'INICIO DESTINO'\nExemplo: A D\nDigite:");
    scanf(" %c %c", &start_char, &end_char);

    // Converte letras para índices
    int inicio = toupper(start_char) - 'A';
    int fim = toupper(end_char) - 'A';

    // Validação básica antes de chamar a função
    if (inicio >= 0 && inicio < num_vertices && fim >= 0 && fim < num_vertices) {
        dijkstra(matriz, num_vertices, inicio, fim);
    } else {
        printf("Erro: Vertices invalidos!\n");
    }

    return 0;
}


/*--=======================--
 *        Funcoes
 *--=======================--*/

void dijkstra(int grafo[MAX_VERTICES][MAX_VERTICES], int num_vertices, int inicio, int fim) {
    int distancia[MAX_VERTICES];
    int visitado[MAX_VERTICES];
    int pai[MAX_VERTICES]; // Armazena o vertice anterior para reconstruir o caminho

    // inicializacao
    for (int i = 0; i < num_vertices; i++) {
        distancia[i] = INFINITO; // "nao percorri ninguem ainda"
        visitado[i] = 0;         // "nao visitei ninguem ainda" 0 = Aberto, 1 = Fechado
        pai[i] = -1;             // "nao tem nenhuma ligacao ainda"
    }
    distancia[inicio] = 0; // a distância para si mesmo é zero

    // processamento
    for (int i = 0; i < num_vertices; i++) {

        // busca vertice que nao foi visitado com menor distancia / "Escolha gulosa"
        int atual = -1;
        int menor_dist = INFINITO;

        for (int vizinho = 0; vizinho < num_vertices; vizinho++) {
            if (visitado[vizinho] == 0 && distancia[vizinho] < menor_dist) {
                menor_dist = distancia[vizinho];
                atual = vizinho;
            }
        }

        // se nao achou ninguem ou é infinito, para o for
        if (atual == -1 || distancia[atual] == INFINITO) break;

        // marca visitado (fechado)
        visitado[atual] = 1;

        // para quando chega no destino
        if (atual == fim) break;

        // comeca a busca nos seus vizinhos / "relaxamento"
        for (int vizinho = 0; vizinho < num_vertices; vizinho++) {

            // se existe aresta (peso > 0) E o vizinho não foi visitado
            if (grafo[atual][vizinho] > 0 && visitado[vizinho] == 0) {

                int nova_distancia = distancia[atual] + grafo[atual][vizinho]; // vizinho + atual

                // se achou um caminho mais curto, entao atualiza a distancia
                if (nova_distancia < distancia[vizinho]) {
                    distancia[vizinho] = nova_distancia;
                    pai[vizinho] = atual; // O pai de 'vizinho' agora é 'atual'
                }
            }
        }
    }

    // mostra o resultado
    printf("\n--- Resultado ---\n");
    if (distancia[fim] == INFINITO) {
        printf("Nao existe caminho entre %c e %c.\n", 'A' + inicio, 'A' + fim);
    } else {
        printf("Custo total: %d\n", distancia[fim]);
        printf("Caminho: ");

        // caminho esta de tras para frente no vetor pai
        int caminho[MAX_VERTICES];
        int cont = 0;
        int atual = fim;

        while (atual != -1) {
            caminho[cont] = atual;
            cont++;
            atual = pai[atual];
        }

        // imprime o vetor ao contrário
        for (int k = cont - 1; k >= 0; k--) {
            printf("%c", 'A' + caminho[k]);
            if (k > 0) printf(" -> ");
        }
    }
}


// =====================================================


void imprimirGrafo(int matriz[MAX_VERTICES][MAX_VERTICES], int num_vertices) {

    limpaTela();
    printf("\nMatriz adjacente pesos:\n");

    // coloca o cabecalho, com 3 de espaco entre colunas + 1 (total 4) pra proxima coluna
    for (int i = 0; i < num_vertices; i++) {
        if (i == 0) printf("    ");
        printf("%c   ", 'A' + i);
        if (i+1 == num_vertices) printf("\n"); // qnd for o ultimo da \n
    }

    // monta as linhas
    for (int i = 0; i < num_vertices; i++) {
        printf("%c:  ", 'A' + i);

        for (int j = 0; j < num_vertices; j++) {
            // if !vazio?
            if (matriz[i][j] != 0) {
                // -3 pra ficar alinhado os caracteres qnd for mto grande o num
                printf("%-3d ", matriz[i][j]);
            } else {
                printf("-   ");
            }
        }
        printf("\n");
    }
    printf("\n");
}


// =====================================================

void limpaTela() {
    system("cls || clear");
}
