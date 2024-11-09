#ifndef AUX_H
#define AUX_H

#include <stdint.h>

// cria struct com o morto/vivo e a quantidade de vizinhos vivos?

uint8_t** alocaMatriz(int m, int n);

void read_tabuleiro(uint8_t **tabuleiro, int m, int n);

void print_tabuleiro(uint8_t **tabuleiro, int m, int n);

void destroy_tabuleiro(uint8_t **tabuleiro, int m);

#endif