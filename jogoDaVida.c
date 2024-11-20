#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "aux.h"

int nosExplorados = 0;

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
            return 0;
        if(qtde_vizinhos == 2 && estado_atual[m_superior_esquerdo][n_superior_esquerdo] == 1) 
            return 0;
    }
    

    // int inicio_m = mAtual-1;
    // int inicio_n = nAtual-1;
    // int fim_m = mAtual;
    // int fim_n = nAtual+1;

    // if(mAtual-1 < 0)
    //     inicio_m = 0;
    // if(nAtual-1 < 0) 
    //     inicio_n = 0;
    // if(nAtual+1 > n)
    //     fim_n = nAtual;

    // for(int i = inicio_m; i <= mAtual; i++) {
    //     for(int j = inicio_n; j <= nAtual+1; j++) {
    //         if(i == mAtual && j == nAtual+1) // celula nao preenchida
    //             return 1;

    //         int qtde_vizinhos = qtdeVizinhosVivos(estado_atual, m, n, i, j);
    //         if(prox_estado[i][j] == 1) {
    //             if ((qtde_vizinhos == 2) && (estado_atual[i][j] != 1) )
    //                 return 0;
    //             if(qtde_vizinhos < 2 || qtde_vizinhos > 3) 
    //                 return 0;
    //         } else {
    //             if(qtde_vizinhos == 3) 
    //                 return 0;
    //             if(qtde_vizinhos == 2 && estado_atual[i][j] == 1) 
    //                 return 0;
    //         }
    //     }
    // }
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
            printf("Atualizando o valor da %d %d: %hhd\n", i, j, qtde_vizinhos[i][j]);
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

void progride(uint8_t **estado_atual, uint8_t **prox_estado, uint8_t **qtde_vizinhos, int m, int n, int mAtual, int nAtual) {
    nosExplorados++;
    if (mAtual == m) {
        printf("\nResultado:\n");
        print_tabuleiro(estado_atual, m, n);
        // printf("Vizinhos:\n");
        // print_tabuleiro(qtde_vizinhos, m, n);
        printf("Vivos: %d\n", num_vivos(estado_atual, m, n));
        printf("\n\n");
        return;
    }
    // TODO: armazenar melhor resultado de vivos e se or maior, podar
    // Tem resultado errado ainda
    // verificar se é possível retirar umas copias de matriz

    int prox_m = mAtual;
    int prox_n = nAtual;
    if (nAtual == n-1) {
        prox_n = 0;
        prox_m++;
    } else {
        prox_n++;
    }

    int qtde_vizinhos_atual = qtdeVizinhosVivos(prox_estado, m, n, mAtual, nAtual);

    uint8_t **vizinhosTemp = alocaMatriz(m, n);
    copiarMatriz(qtde_vizinhos, vizinhosTemp, m, n);

    // para minimizar a quantidade de células vivas - poda
    if((qtde_vizinhos_atual == 0) && (prox_estado[mAtual][nAtual] == 0)) {
        estado_atual[mAtual][nAtual] = 0;

        printf("Estado:\n");
        print_tabuleiro(estado_atual, m, n);
        printf("ehEstadoPossivel: %d\n", ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual));

        if(ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual)) {
            progride(estado_atual, prox_estado, vizinhosTemp, m, n, prox_m, prox_n);
            copiarMatriz(qtde_vizinhos, vizinhosTemp, m, n);
        }
    } else {
        estado_atual[mAtual][nAtual] = 0;

        printf("Estado:\n");
        print_tabuleiro(estado_atual, m, n);
        printf("ehEstadoPossivel: %d\n", ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual));

        if(ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual)) {
            progride(estado_atual, prox_estado, vizinhosTemp, m, n, prox_m, prox_n);
            copiarMatriz(qtde_vizinhos, vizinhosTemp, m, n);
        }

        estado_atual[mAtual][nAtual] = 1;

        printf("Estado:\n");
        print_tabuleiro(estado_atual, m, n);
        printf("ehEstadoPossivel: %d\n", ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual));


        if(ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual)) {
            aumentaVizinhosVivos(qtde_vizinhos, m, n, mAtual, nAtual);
            printf("Qtde vizinhos vivos:\n");
            print_tabuleiro(qtde_vizinhos, m, n);
            printf("\n\n");
            copiarMatriz(qtde_vizinhos, vizinhosTemp, m, n);
            progride(estado_atual, prox_estado, vizinhosTemp, m, n, prox_m, prox_n);
            copiarMatriz(qtde_vizinhos, vizinhosTemp, m, n);
        }
    }
}

int main(int argc, char *argv[]) {
    int linhas, colunas;
 
    scanf("%d %d", &linhas, &colunas);
    uint8_t **tabuleiro = alocaMatriz(linhas, colunas);
    read_tabuleiro(tabuleiro, linhas, colunas);

    uint8_t **tabuleiro_resultado = alocaMatriz(linhas, colunas);

    uint8_t **qtde_vizinhos = (uint8_t **)calloc(linhas, sizeof(uint8_t *));
    for (uint8_t i = 0; i < linhas; i++) 
        qtde_vizinhos[i] = (uint8_t *)calloc(colunas, sizeof(uint8_t));
    
    progride(tabuleiro_resultado, tabuleiro, qtde_vizinhos, linhas, colunas, 0, 0);
    
    printf("Nos explorados: %d\n", nosExplorados);
    destroy_tabuleiro(tabuleiro, linhas);
    destroy_tabuleiro(tabuleiro_resultado, linhas);
    destroy_tabuleiro(qtde_vizinhos, linhas);
    return 0;
}