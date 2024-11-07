#ifndef AUX_H
#define AUX_H

// cria struct com o morto/vivo e a quantidade de vizinhos vivos?

int** alocaMatriz(int m, int n);

void read_tabuleiro(int **tabuleiro, int m, int n);

void print_tabuleiro(int **tabuleiro, int m, int n);

void destroy_tabuleiro(int **tabuleiro, int m);

#endif