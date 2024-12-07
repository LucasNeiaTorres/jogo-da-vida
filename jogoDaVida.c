#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "aux.h"

#define INT_MAX 625

uint8_t **melhor_resultado = NULL;
int menor_qtde_vivos = INT_MAX;
int nosExplorados = 0;
int qtde_vivos_prox;

uint8_t** minTabuleiro(uint8_t** tabuleiroA, uint8_t** tabuleiroB, int m, int n) {
    int vivosA = 0;
    int vivosB = 0;
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(tabuleiroA[i][j] == 1) 
                vivosA++;
            if(tabuleiroB[i][j] == 1) 
                vivosB++;
        }   
    }
    if(tabuleiroA < tabuleiroB) 
        return tabuleiroA;
    return tabuleiroB;
}

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

int qtdeCelulasVivas(uint8_t** tabuleiro, int m, int n) {
    int vivos = 0;
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(tabuleiro[i][j] == 1) 
                vivos++;
        }   
    }
    return vivos;
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

int ehEstadoPossivel(uint8_t **estado_atual, uint8_t **prox_estado, int m, int n, int mAtual, int nAtual) {
    int n_superior_esquerdo = nAtual-1;
    int m_superior_esquerdo = mAtual-1;
    
    if(nAtual-1 < 0 || mAtual-1 < 0)
        return 1;

    int qtde_vizinhos = qtdeVizinhosVivos(estado_atual, m, n, m_superior_esquerdo, n_superior_esquerdo);
    if(prox_estado[m_superior_esquerdo][n_superior_esquerdo] == 1) {
        if ((qtde_vizinhos == 2) && (estado_atual[m_superior_esquerdo][n_superior_esquerdo] != 1) )
            return 0;
        if(qtde_vizinhos < 2 || qtde_vizinhos > 3) 
            return 0;
    } else {
        if(qtde_vizinhos == 3) 
            return 0; // nao pode renascer
        if(qtde_vizinhos == 2 && estado_atual[m_superior_esquerdo][n_superior_esquerdo] == 1) 
            return 0; // nao pode permanecer viva
    }

    return 1;
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

int num_vivos(uint8_t **tabuleiro, int m, int n) {
    int vivos = 0;
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++) 
            if (tabuleiro[i][j] == 1) 
                vivos++;
    return vivos;
}

int ehVizinhosPossivel(uint8_t **estado_atual, uint8_t **prox_estado, uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual) {
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

    // printf("inicio_m: %d | fim_m: %d\n", inicio_m, fim_m);
    // percorre todos os vizinhos
    for(int i = inicio_m; i <= fim_m; i++) {
        for(int j = inicio_n; j <= fim_n; j++) {
            // verifica se eh celula nao preenchida
            if(i == mAtual && j == nAtual+1) 
                return 1;
                
            // printf("mAtual: %d | nAtual+1: %d\n", mAtual, nAtual+1);
            // printf("i: %d | j: %d\n", i, j);
            // verifica se eh o bloco finalizado e se o numero de vivos esta correto
            // se for borda direita entao o bloco esta finalizado tambem
            if(((i == inicio_m) && (j == inicio_n)) || ((ehBorda(prox_estado, m, n, i, j) == 3) && (j == nAtual))) {
                if(prox_estado[i][j] == 1) {
                    if((qtde_vizinhos[i][j] == 2) && (estado_atual[i][j] != 1)) {
                        // printf("corte 1\n");
                        // printf("qtde_vizinhos[%d][%d]: %d (==2)\n", i, j, qtde_vizinhos[i][j]);
                        // printf("estado_atual[%d][%d]: %d (!=1)\n\n", i, j, estado_atual[i][j]);
                        return 0;
                    }
                    if(qtde_vizinhos[i][j] < 2 || qtde_vizinhos[i][j] > 3)  {
                        // printf("corte 2\n");    
                        // printf("qtde_vizinhos[%d][%d]: %d (<2 ou >3)\n\n", i, j, qtde_vizinhos[i][j]);
                        return 0;
                    }
                } else {
                    if(qtde_vizinhos[i][j] == 3) {
                        // printf("corte 3\n");
                        // printf("qtde_vizinhos[%d][%d]: %d (==3)\n\n", i, j, qtde_vizinhos[i][j]);
                        return 0;
                    }
                    if(qtde_vizinhos[i][j] == 2 && estado_atual[i][j] == 1) {
                        // printf("corte 4\n");
                        // printf("qtde_vizinhos[%d][%d]: %d (==2)\n", i, j, qtde_vizinhos[i][j]);
                        // printf("estado_atual[%d][%d]: %d (==1)\n\n", i, j, estado_atual[i][j]);
                        return 0;
                    }
                }
                continue;
            }
            // printf("qtde_vizinhos[%d][%d]: %d | prox_estado = %d\n", i, j, qtde_vizinhos[i][j], prox_estado[i][j]);
            // verifica se ja passou do numero de vivos, se proximo estado for 1
            if((prox_estado[i][j] == 1) && (qtde_vizinhos[i][j] > 3)) {
                // printf("corte 5\n");
                // printf("qtde_vizinhos[%d][%d]: %d (>3)\n\n", i, j, qtde_vizinhos[i][j]);
                // printf("prox_estado[%d][%d]: %d (==1)\n\n", i, j, prox_estado[i][j]);
                return 0;
            }
            // verifica, caso proximo seja 1, se o numero de vivos eh menor que 2 ou 3 e falta menos de 2 ou 3 celulas pra completar o bloco dele
            // calcula quantas celulas falta pra completar o bloco considerando o mAtual e o nAtual

            // calcula celulas faltantes da celula[i][j]
            // exemplo:
            // 0 0 0 0 0
            // 0 0 .0. 1 0 <- celula[i][j] entre pontos
            // 0 0 (0) x x <- celula[mAtual][nAtual] entre parenteses e x ainda não definido
            // sabendo que o proximo estado de celula[i][j] = 1
            // celula[mAtual][nAtual] tem que ser 1 pois celulas_faltantes = 1 e qtde_vizinhos[i][j] = 1
            // logo, celula[mAtual][nAtual] = 1

            // formula eh: abs(l-lA)xnL + abs(c-cA)
            // dado que lA = mAtual, cA = nAtual
            // l,c eh coordenada da ultima celula do bloco (i+1, j+1)
            // ehBorda esquerda (mAtual, NAtual): nL = 2
            if(prox_estado[i][j] == 1) {
                int linha_final = i+1;
                int coluna_final = j+1;
                int nL = 3;
                // se for borda direita
                if(ehBorda(prox_estado, m, n, mAtual, nAtual) == 3) {
                    linha_final = i+1;
                    coluna_final = j;
                    // celulas_faltantes = 0; pensar no celula atual
                } else if (ehBorda(prox_estado, m, n, mAtual, nAtual) == 4) {
                    linha_final = i;
                    coluna_final = j+1;
                } else if(ehBorda(prox_estado, m, n, mAtual, nAtual) == 1) {
                    nL = 2;
                }

                int celulas_faltantes = abs(linha_final - mAtual) * nL + abs(coluna_final - nAtual);
                // printf("[%d][%d] Celulas faltantes: %d\n", i, j, celulas_faltantes);
                // calcula celulas faltantes da celula[i][j]

                if(estado_atual[i][j] == 0) {
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

int ehRespostaValida(uint8_t **estado_atual, uint8_t **prox_estado, uint8_t **qtde_vizinhos, int m, int n) {
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            if(prox_estado[i][j] == 1) {
                if((qtde_vizinhos[i][j] == 2) && (estado_atual[i][j] != 1)) 
                    return 0;
                if(qtde_vizinhos[i][j] < 2 || qtde_vizinhos[i][j] > 3) 
                    return 0;
            } else {
                if(qtde_vizinhos[i][j] == 3) 
                    return 0;
                if(qtde_vizinhos[i][j] == 2 && estado_atual[i][j] == 1) 
                    return 0;
            }
        }
    }

    return 1;
}

int qtde_vivos(uint8_t **tabuleiro, int m, int n) {
    int vivos = 0;
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++) 
            if (tabuleiro[i][j] == 1) 
                vivos++;
    return vivos;
}

void progride(uint8_t **estado_atual, uint8_t **prox_estado, uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual, int vivos_atual) {
    nosExplorados++;

    // if(nosExplorados >= 100000000000) { // faz ele ser proporcional ao tam do tabuleiro
    //     return;
    // }

    // pula borda
    int prox_m = (nAtual == n-1) ? mAtual + 1 : mAtual;
    int prox_n = (nAtual == n-1) ? 0 : nAtual + 1;

    // armazena nessa variavel global
    if(prox_m == m){
        printf("Resultado: %d\n", ehRespostaValida(estado_atual, prox_estado, qtde_vizinhos, m, n));
        printf("Vivos: %d\n", vivos_atual);
        print_tabuleiro(estado_atual, m, n);
        printf("\n\n");
        // quando tiver umas poda foda da pra comentar essa linha
        if(ehRespostaValida(estado_atual, prox_estado, qtde_vizinhos, m, n) != 0){
            if(vivos_atual < menor_qtde_vivos){
                menor_qtde_vivos = vivos_atual;

                for(int i = 0; i < m; i++){
                    for(int j = 0; j < n; j++){
                        melhor_resultado[i][j] = estado_atual[i][j];
                    }
                }
            }
        }
        return;
    }


    // PODA - se passar o numero de vivos do melhor local nem tenta mais 
    // essa poda melhorou 87% a busca -> 24k para 3k de nós - proporcional a quantidade de celulas mortas sem vizinhos
    if (vivos_atual >= menor_qtde_vivos) {
        return;
    }

    
    // PODA - Se a celula ta morta no prox estado e nao tem vizinhos vivos nao precisa testar
    if (prox_estado[mAtual][nAtual] == 0 && qtdeVizinhosVivos(prox_estado, m, n, mAtual, nAtual) == 0) {
        estado_atual[mAtual][nAtual] = 0;
        // printf("Poda 1\n");
        progride(estado_atual, prox_estado, qtde_vizinhos, m, n, prox_m, prox_n, vivos_atual);
        return;
    }

    int qtde_vizinhos_prox = qtdeVizinhosVivos(prox_estado, m, n, mAtual, nAtual);

    int prioridade[2] = {0, 1};

    // if((prox_estado[mAtual][nAtual] == 1) && (qtde_vizinhos_prox >= 2) 
    // || ((prox_estado[mAtual][nAtual] == 0) && (qtde_vizinhos_prox >= 3))
    // ) { 
    //     prioridade[0] = 1;
    //     prioridade[1] = 0;
    // }
    int vivos = vivos_atual;

    // implementa fila de prioridade
    for(int i = 0; i < 2; i++) {
        estado_atual[mAtual][nAtual] = prioridade[i];
        if(prioridade[i] == 1) {
            aumentaVizinhosVivos(qtde_vizinhos, m, n, mAtual, nAtual);
            vivos++;
        }
        if (ehVizinhosPossivel(estado_atual, prox_estado, qtde_vizinhos, m, n, mAtual, nAtual) == 1) {
            progride(estado_atual, prox_estado, qtde_vizinhos, m, n, prox_m, prox_n, vivos);
        }
        if(prioridade[i] == 1) {
            estado_atual[mAtual][nAtual] = 0;
            diminuiVizinhosVivos(qtde_vizinhos, m, n, mAtual, nAtual);
            vivos--;    
        }
    }
}


int main(int argc, char *argv[]) {
    int linhas, colunas;
 
    scanf("%d %d", &linhas, &colunas);
    uint8_t **tabuleiro = alocaMatriz(linhas, colunas);
    read_tabuleiro(tabuleiro, linhas, colunas);


    uint8_t **tabuleiro_resultado = alocaMatriz(linhas, colunas);
    melhor_resultado = alocaMatriz(linhas, colunas);

    uint8_t **qtde_vizinhos = (uint8_t **)calloc(linhas, sizeof(uint8_t *));
    for (uint8_t i = 0; i < linhas; i++) 
        qtde_vizinhos[i] = (uint8_t *)calloc(colunas, sizeof(uint8_t));
    
    qtde_vivos_prox = qtde_vivos(tabuleiro, linhas, colunas);
    menor_qtde_vivos = qtde_vivos_prox * 2; // impossível ter mais que 3 vezes a quantidade de vivos - calcular densidade de vivos
    // menor_qtde_vivos = linhas * colunas;

    progride(tabuleiro_resultado, tabuleiro, qtde_vizinhos, linhas, colunas, 0, 0, 0);
    
    printf("\nMelhor Resultado:\n");
    printf("%d %d\n", linhas, colunas);
    print_tabuleiro(melhor_resultado, linhas, colunas);
    printf("Quantidade de vivos: %d\n", menor_qtde_vivos);
    printf("Nos explorados: %d\n", nosExplorados);
    destroy_tabuleiro(tabuleiro, linhas);
    destroy_tabuleiro(tabuleiro_resultado, linhas);
    destroy_tabuleiro(qtde_vizinhos, linhas);
    return 0;
}