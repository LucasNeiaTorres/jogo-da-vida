#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include "aux.h"
#define INT_MAX 625
#define TAM_BLOCO 5

uint8_t **melhor_resultado = NULL;
int menor_qtde_vivos = INT_MAX;
int nosExplorados = 0;
int qtde_vivos_prox;

// retorna 1 para canto superior esquerdo, 
// 2 para canto superior direito, 
// 3 para canto inferior esquerdo, 
// 4 para canto inferior direito e 
// 0 se não for canto
int ehCanto(uint8_t** tabuleiro, int m, int n, int i, int j) {
    if(i == 0 && j == 0) 
        return 1;
    if(i == 0 && j == n-1) 
        return 2;
    if(i == m-1 && j == 0) 
        return 3;
    if(i == m-1 && j == n-1) 
        return 4;
    return 0;
}

// retorna 1 se for borda esquerda,
// 2 se for borda superior,
// 3 se for borda direita,
// 4 se for borda inferior
// 0 se não for borda
int ehBorda(uint8_t** tabuleiro, int m, int n, int i, int j) {
    if(i == 0) 
        return 2;
    if(i == m-1) 
        return 4;
    if(j == 0) 
        return 1;
    if(j == n-1) 
        return 3;
    return 0;
}

uint8_t qtdeVizinhosVivos(uint8_t** tabuleiro, int m, int n, int i, int j) {
    int posicao = ehCanto(tabuleiro, m, n, i, j);
    // tratamento de canto
    if(posicao == 1) 
        return tabuleiro[i][j+1] + tabuleiro[i+1][j] + tabuleiro[i+1][j+1];
    else if(posicao == 2) 
        return tabuleiro[i][j-1] + tabuleiro[i+1][j] + tabuleiro[i+1][j-1];
    else if(posicao == 3) 
        return tabuleiro[i][j+1] + tabuleiro[i-1][j] + tabuleiro[i-1][j+1];
    else if(posicao == 4)
        return tabuleiro[i][j-1] + tabuleiro[i-1][j] + tabuleiro[i-1][j-1];
    
    // tratamento de borda
    int borda = ehBorda(tabuleiro, m, n, i, j);
    if(borda == 1) 
        return tabuleiro[i-1][j] + tabuleiro[i+1][j] + tabuleiro[i-1][j+1] + tabuleiro[i][j+1] + tabuleiro[i+1][j+1];
    else if(borda == 2)
        return tabuleiro[i][j-1] + tabuleiro[i][j+1] + tabuleiro[i+1][j-1] + tabuleiro[i+1][j] + tabuleiro[i+1][j+1];
    else if(borda == 3)
        return tabuleiro[i-1][j] + tabuleiro[i+1][j] + tabuleiro[i-1][j-1] + tabuleiro[i][j-1] + tabuleiro[i+1][j-1];
    else if(borda == 4)
        return tabuleiro[i][j-1] + tabuleiro[i][j+1] + tabuleiro[i-1][j-1] + tabuleiro[i-1][j] + tabuleiro[i-1][j+1];
    
    // se não for canto nem borda
    return tabuleiro[i-1][j-1] + tabuleiro[i-1][j] + tabuleiro[i-1][j+1] + tabuleiro[i][j-1] + tabuleiro[i][j+1] + tabuleiro[i+1][j-1] + tabuleiro[i+1][j] + tabuleiro[i+1][j+1];
}

// funcao chamada quando adiciona uma celula viva
// atualiza a quantidade de vizinhos vivos de todas as celulas vizinhas da celula adicionada
void aumentaVizinhosVivos(uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual) {
    int inicio_m = mAtual-1;
    int inicio_n = nAtual-1;
    int fim_m = mAtual+1;
    int fim_n = nAtual+1;

    if(mAtual-1 < 0)
        inicio_m = 0;
    if(nAtual-1 < 0) 
        inicio_n = 0;
    if(nAtual+1 >= n)
        fim_n = nAtual;
    if(mAtual+1 >= m)
        fim_m = mAtual;


    for(int i = inicio_m; i <= fim_m; i++) {
        for(int j = inicio_n; j <= fim_n; j++) {
            if(i == mAtual && j == nAtual) // celula adicionada
                continue;
            qtde_vizinhos[i][j]++;
            //printf("Atualizando o valor da %d %d: %hhd\n", i, j, qtde_vizinhos[i][j]);
        }
    }
}

// funcao chamada quando adiciona uma celula morta
// atualiza a quantidade de vizinhos vivos de todas as celulas vizinhas da celula adicionada
void diminuiVizinhosVivos(uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual) {
    int inicio_m = mAtual - 1;
    int inicio_n = nAtual - 1;
    int fim_m = mAtual + 1;
    int fim_n = nAtual + 1;

    if (inicio_m < 0)  
        inicio_m = 0;
    if (inicio_n < 0) 
        inicio_n = 0;
    if (fim_m >= m) 
        fim_m = mAtual;
    if (fim_n >= n) 
        fim_n = nAtual;

    for (int i = inicio_m; i <= fim_m; i++) {
        for (int j = inicio_n; j <= fim_n; j++) {
            if (i == mAtual && j == nAtual) 
                continue;
            qtde_vizinhos[i][j]--;
        }
    }
}

int ehVizinhosPossivel(uint8_t **estado_anterior, uint8_t **estado_dado, uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual) {
    int inicio_m = mAtual - 1;
    int inicio_n = nAtual - 1;
    int fim_m = mAtual;
    int fim_n = nAtual + 1;
    
    // primeira linha do tabuleiro
    if(mAtual == 0) {
        return 1;
    }

    // tratamento de borda
    if(mAtual - 1 < 0) 
        inicio_m = 0;
    if(nAtual - 1 < 0)
        inicio_n = 0;
    if(nAtual + 1 >= n)
        fim_n = nAtual;

    // percorre todos os vizinhos
    for(int i = inicio_m; i <= fim_m; i++) {
        for(int j = inicio_n; j <= fim_n; j++) {
            // verifica se eh celula nao preenchida
            if(i == mAtual && j == nAtual+1) 
                return 1;

            // verifica se eh o bloco finalizado e se o numero de vivos esta correto
            // se for borda direita entao o bloco esta finalizado tambem
            if((((i == inicio_m) && (j == inicio_n))
            || (((ehBorda(estado_dado, m, n, i, j) == 3) || (ehCanto(estado_dado, m, n, i, j) == 2)) && (j == nAtual))
            || (ehBorda(estado_dado, m, n, i, j) == 4))
            ) {
                // printf("Bloco finalizado\n");
                if(estado_dado[i][j] == 1) {
                    if((qtde_vizinhos[i][j] == 2) && (estado_anterior[i][j] != 1)) {
                        // printf("corte 1\n");
                        return 0;
                    }
                    if(qtde_vizinhos[i][j] < 2 || qtde_vizinhos[i][j] > 3)  {
                        // printf("corte 2\n"); 
                        return 0;
                    }
                } else {
                    if(qtde_vizinhos[i][j] == 3) {
                        // printf("corte 3\n");
                        return 0;
                    }
                    if(qtde_vizinhos[i][j] == 2 && estado_anterior[i][j] == 1) {
                        // printf("corte 4\n");
                        return 0;
                    }
                }
                continue;
            }
            // printf("qtde_vizinhos[%d][%d]: %d | estado_dado = %d\n", i, j, qtde_vizinhos[i][j], estado_dado[i][j]);
            // verifica se ja passou do numero de vivos, se proximo estado for 1
            if((estado_dado[i][j] == 1) && (qtde_vizinhos[i][j] > 3)) {
                // printf("corte 5\n");
                // printf("qtde_vizinhos[%d][%d]: %d (>3)\n\n", i, j, qtde_vizinhos[i][j]);
                // printf("estado_dado[%d][%d]: %d (==1)\n\n", i, j, estado_dado[i][j]);
                return 0;
            }
            // verifica, caso proximo seja 1, se o numero de vivos eh menor que 2 ou 3 e falta menos de 2 ou 3 celulas pra completar o bloco dele
            // calcula quantas celulas falta pra completar o bloco considerando o mAtual e o nAtual
            if(estado_dado[i][j] == 1) {
                int linha_final = i+1;
                int coluna_final = j+1;
                int nL = 3;
                // se for borda direita
                if(ehBorda(estado_dado, m, n, mAtual, nAtual) == 3) {
                    linha_final = i+1;
                    coluna_final = j;
                    // celulas_faltantes = 0; pensar no celula atual
                } else if (ehBorda(estado_dado, m, n, mAtual, nAtual) == 4) {
                    linha_final = i;
                    coluna_final = j+1;
                } else if(ehBorda(estado_dado, m, n, mAtual, nAtual) == 1) {
                    nL = 2;
                }

                // calcula celulas faltantes da celula[i][j]
                int celulas_faltantes = abs(linha_final - mAtual) * nL + abs(coluna_final - nAtual);

                if(estado_anterior[i][j] == 0) {
                    if((celulas_faltantes + qtde_vizinhos[i][j]) < 3) {
                        // printf("corte 6\n");
                        return 0;
                    }
                } else {
                    if((celulas_faltantes + qtde_vizinhos[i][j]) < 2) {
                        // printf("corte 7\n");
                        return 0;
                    }
                }
            }
        }
    }
    // verifica se eh atual
    return 1;
}

int ehRespostaValida(uint8_t **estado_anterior, uint8_t **estado_dado, uint8_t **qtde_vizinhos, int m, int n) {
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(estado_dado[i][j] == 1) {
                if((qtde_vizinhos[i][j] == 2) && (estado_anterior[i][j] != 1)) {
                    printf("Erro 1 na celula %d %d\n", i, j);
                    return 0;
                }
                if(qtde_vizinhos[i][j] < 2 || qtde_vizinhos[i][j] > 3) {
                    printf("Erro 2 na celula %d %d\n", i, j);
                    return 0;
                }
            } else {
                if(qtde_vizinhos[i][j] == 3) {
                    printf("Erro 3 na celula %d %d\n", i, j);
                    return 0;
                }
                if(qtde_vizinhos[i][j] == 2 && estado_anterior[i][j] == 1) {
                    printf("Erro 4 na celula %d %d\n", i, j);
                    return 0;
                }
            }
        }
    }

    return 1;
}

int qtdeVivosEstadoDado(uint8_t **tabuleiro, int m, int n) {
    int vivos = 0;
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++) 
            if (tabuleiro[i][j] == 1) 
                vivos++;
    return vivos;
}


// a probabilidade da quantidade de vivos ser maior que o menor qtde_vivos proporcionamente a quantidade de celulas faltantes do tabuleiro
bool heuristicaVivosProporcional(uint8_t **estado_anterior, uint8_t **estado_dado, uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual, int vivos_atual) {
    int celulas_faltantes = 0;
    // calcula a probabilidade de ser maior
    for(int i = mAtual; i < m; i++) {
        for(int j = nAtual; j < n; j++) {
            if(estado_dado[i][j] == 1) {
                if((qtde_vizinhos[i][j] == 2) && (estado_anterior[i][j] != 1)) {
                    celulas_faltantes++;
                }
                if(qtde_vizinhos[i][j] < 2 || qtde_vizinhos[i][j] > 3) {
                    celulas_faltantes++;
                }
            } else {
                if(qtde_vizinhos[i][j] == 3) {
                    celulas_faltantes++;
                }
                if(qtde_vizinhos[i][j] == 2 && estado_anterior[i][j] == 1) {
                    celulas_faltantes++;
                }
            }
        }
    }
    
    return (vivos_atual + celulas_faltantes) < menor_qtde_vivos;
}

void progride(uint8_t **estado_anterior, uint8_t **estado_dado, uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual, int vivos_atual) {
    nosExplorados++;

    if(nosExplorados >= 1000000000) { // faz ele ser proporcional ao tam do tabuleiro
        return;
    }
    
    // pula borda
    int prox_m = (nAtual == n-1) ? mAtual + 1 : mAtual;
    int prox_n = (nAtual == n-1) ? 0 : nAtual + 1;

    // armazena nessa variavel global
    if(prox_m == m){
        printf("Resultado: %d\n", ehRespostaValida(estado_anterior, estado_dado, qtde_vizinhos, m, n));
        printf("Vivos: %d\n", vivos_atual);
        print_tabuleiro(estado_anterior, m, n);
        printf("\n\n");
        if(vivos_atual < menor_qtde_vivos){
            if(ehRespostaValida(estado_anterior, estado_dado, qtde_vizinhos, m, n) != 0){
                menor_qtde_vivos = vivos_atual;

                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n; j++){
                        melhor_resultado[i][j] = estado_anterior[i][j];
                    }
                }
            }
        }
        return;
    }

    // PODA - se passar o numero de vivos do melhor local nem tenta mais 
    if (vivos_atual >= menor_qtde_vivos) {
        return;
    }

    if(!(heuristicaVivosProporcional(estado_anterior, estado_dado, qtde_vizinhos, m, n, mAtual, nAtual, vivos_atual))) {
        return;
    }

    // PODA - Se a celula ta morta no estado dado e nao tem vizinhos vivos ela eh morta no anterior
    if (estado_dado[mAtual][nAtual] == 0 && qtdeVizinhosVivos(estado_dado, m, n, mAtual, nAtual) == 0) {
        estado_anterior[mAtual][nAtual] = 0;

        if(ehVizinhosPossivel(estado_anterior, estado_dado, qtde_vizinhos, m, n, mAtual, nAtual) == 1) {
            progride(estado_anterior, estado_dado, qtde_vizinhos, m, n, prox_m, prox_n, vivos_atual);
        }
        return;
    }


    int prioridade[2] = {0, 1};

    int vivos = vivos_atual;

    // implementa fila de prioridade
    for(int i = 0; i < 2; i++){
        estado_anterior[mAtual][nAtual] = prioridade[i];
        if(prioridade[i] == 1){
            aumentaVizinhosVivos(qtde_vizinhos, m, n, mAtual, nAtual);
            vivos++;
        }

        if (ehVizinhosPossivel(estado_anterior, estado_dado, qtde_vizinhos, m, n, mAtual, nAtual) == 1) {
            // pula celulas mortas sem vizinhos vivos
            progride(estado_anterior, estado_dado, qtde_vizinhos, m, n, prox_m, prox_n, vivos);
        }

        if(prioridade[i] == 1){
            estado_anterior[mAtual][nAtual] = 0;
            diminuiVizinhosVivos(qtde_vizinhos, m, n, mAtual, nAtual);
            vivos--;    
        }
    }
}


int main(int argc, char *argv[]) {
    int linhas, colunas;
 
    scanf("%d %d", &linhas, &colunas);
    uint8_t **estado_dado = alocaMatriz(linhas, colunas);
    le_entrada(estado_dado, linhas, colunas);


    uint8_t **resultado_estado_anterior = alocaMatriz(linhas, colunas);
    melhor_resultado = alocaMatriz(linhas, colunas);

    uint8_t **qtde_vizinhos = (uint8_t **)calloc(linhas, sizeof(uint8_t *));
    for(uint8_t i = 0; i < linhas; i++) 
        qtde_vizinhos[i] = (uint8_t *)calloc(colunas, sizeof(uint8_t));
    
    qtde_vivos_prox = qtdeVivosEstadoDado(estado_dado, linhas, colunas);
    // assume que o tabuleiro final terá mais 75% de celulas vivas do tabuleiro inicial
    menor_qtde_vivos = qtde_vivos_prox * 1.75; 

    //calcula o tempo
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // chamada  funcao principal
    // resolver_por_blocos(resultado_estado_anterior, estado_dado, qtde_vizinhos, linhas, colunas);
    progride(resultado_estado_anterior, estado_dado, qtde_vizinhos, linhas, colunas, 0, 0, 0);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("Tempo de execução: %lf\n", (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0);
    

    printf("\nMelhor Resultado:\n");
    printf("%d %d\n", linhas, colunas);
    print_tabuleiro(melhor_resultado, linhas, colunas);
    printf("Quantidade de vivos: %d\n", menor_qtde_vivos);
    printf("Nos explorados: %d\n", nosExplorados);
    destroy_tabuleiro(estado_dado, linhas);
    destroy_tabuleiro(resultado_estado_anterior, linhas);
    destroy_tabuleiro(qtde_vizinhos, linhas);
    return 0;
}