#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "aux.h"

int main(int argc, char *argv[]) {
    int linhas, colunas;
 
    scanf("%d %d", &linhas, &colunas);
    int **tabuleiro = alocaMatriz(linhas, colunas);
    read_tabuleiro(tabuleiro, linhas, colunas);
    
    print_tabuleiro(tabuleiro, linhas, colunas);    

    return 0;
}