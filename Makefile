parametrosCompilacao=-Wall -std=c99 -lm
arquivos=jogoDaVida.o
all: jogoDaVida

jogoDaVida: $(arquivos)
	gcc -o jogoDaVida $(arquivos) $(parametrosCompilacao)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch jogoDaVida 

purge: clean
	-rm -f jogoDaVida