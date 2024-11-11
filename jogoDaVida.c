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
    int inicio_m = mAtual-1;
    int inicio_n = nAtual-1;
    int fim_m = mAtual;
    int fim_n = nAtual+1;

    if(mAtual-1 < 0)
        inicio_m = 0;
    if(nAtual-1 < 0) 
        inicio_n = 0;
    if(nAtual+1 > n)
        fim_n = nAtual;
    
    for(int i = inicio_m; i <= mAtual; i++) {
        for(int j = inicio_n; j <= nAtual+1; j++) {
            if(i == mAtual && j == nAtual+1) // celula nao preenchida
                return 1;

            int qtde_vizinhos = qtdeVizinhosVivos(estado_atual, m, n, i, j);
            if(prox_estado[i][j] == 1) {
                if ((qtde_vizinhos == 2) && (estado_atual[i][j] != 1) )
                    return 0;
                if(qtde_vizinhos < 2 || qtde_vizinhos > 3) 
                    return 0;
            } else {
                if(qtde_vizinhos == 3) 
                    return 0;
                if(qtde_vizinhos == 2 && estado_atual[i][j] == 1) 
                    return 0;
            }
        }
    }
    return 1;
}

void backtracking(uint8_t **estado_atual, uint8_t **prox_estado, int m, int n, int mAtual, int nAtual) {
    nosExplorados++;
    if (mAtual == m) {
        printf("\nResultado:\n");
        print_tabuleiro(estado_atual, m, n);
        return;
    }
    int qtde_vizinhos = qtdeVizinhosVivos(prox_estado, m, n, mAtual, nAtual);

    int prox_m = mAtual;
    int prox_n = nAtual;
    if (nAtual == n-1) {
        prox_n = 0;
        prox_m++;
    } else {
        prox_n++;
    }

    // para minimizar a quantidade de células vivas - poda
    // e se for vivo mas sem vizinhos vivos?
    if((qtde_vizinhos == 0) && (prox_estado[mAtual][nAtual] == 0)) {
        estado_atual[mAtual][nAtual] = 0;
        printf("Estado:\n");
        print_tabuleiro(estado_atual, m, n);
        printf("ehEstadoPossivel: %d\n", ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual));
        if(ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual))
            backtracking(estado_atual, prox_estado, m, n, prox_m, prox_n);
    } else {
        estado_atual[mAtual][nAtual] = 1;
        printf("Estado:\n");
        print_tabuleiro(estado_atual, m, n);
        printf("ehEstadoPossivel: %d\n\n", ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual));
        if(ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual))
            backtracking(estado_atual, prox_estado, m, n, prox_m, prox_n);
        estado_atual[mAtual][nAtual] = 0;
        printf("Estado:\n");
        print_tabuleiro(estado_atual, m, n);
        printf("ehEstadoPossivel: %d\n", ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual));
        if(ehEstadoPossivel(estado_atual, prox_estado, m, n, mAtual, nAtual))
            backtracking(estado_atual, prox_estado, m, n, prox_m, prox_n);
    }

}

int main(int argc, char *argv[]) {
    int linhas, colunas;
 
    scanf("%d %d", &linhas, &colunas);
    uint8_t **tabuleiro = alocaMatriz(linhas, colunas);
    read_tabuleiro(tabuleiro, linhas, colunas);

    uint8_t **tabuleiro_resultado = alocaMatriz(linhas, colunas);
    
    backtracking(tabuleiro_resultado, tabuleiro, linhas, colunas, 0, 0);
    
    printf("Nos explorados: %d\n", nosExplorados);
    destroy_tabuleiro(tabuleiro, linhas);
    return 0;
}