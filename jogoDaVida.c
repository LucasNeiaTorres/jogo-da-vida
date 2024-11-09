#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "aux.h"

int main(int argc, char *argv[]) {
    int linhas, colunas;
 
    scanf("%d %d", &linhas, &colunas);
    uint8_t **tabuleiro = alocaMatriz(linhas, colunas);
    read_tabuleiro(tabuleiro, linhas, colunas);
    
    
    return 0;
}