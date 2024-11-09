#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "aux.h"

uint8_t **alocaMatriz(int m, int n) {
    uint8_t **tabuleiro = (uint8_t **)malloc(m * sizeof(uint8_t *));
    for (uint8_t i = 0; i < m; i++) 
        tabuleiro[i] = (uint8_t *)malloc(n * sizeof(uint8_t));
    return tabuleiro;
}

void read_tabuleiro(uint8_t **tabuleiro, int m, int n) {
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++) 
            scanf("%hhd", &tabuleiro[i][j]);
}

void print_tabuleiro(uint8_t **tabuleiro, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) 
            printf("%hhd ", tabuleiro[i][j]);
        printf("\n");
    }
}

void destroy_tabuleiro(uint8_t **tabuleiro, int m) {
    for (int i = 0; i < m; i++) 
        free(tabuleiro[i]);
    free(tabuleiro);
}