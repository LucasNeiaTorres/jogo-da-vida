#include <stdio.h>
#include <stdlib.h>
#include "aux.h"

int **alocaMatriz(int m, int n) {
    int **tabuleiro = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) 
        tabuleiro[i] = (int *)malloc(n * sizeof(int));
    return tabuleiro;
}

void read_tabuleiro(int **tabuleiro, int m, int n) {
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++) 
            scanf("%d", &tabuleiro[i][j]);
}

void print_tabuleiro(int **tabuleiro, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            printf("%d ", tabuleiro[i][j]);
        printf("\n");
    }
}

void destroy_tabuleiro(int **tabuleiro, int m) {
    for (int i = 0; i < m; i++) 
        free(tabuleiro[i]);
    free(tabuleiro);
}