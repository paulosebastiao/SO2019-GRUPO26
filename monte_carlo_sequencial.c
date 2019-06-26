/************************************************************************************************
*			Primeiro Trabalho de Programacao Concorrente				*	
*	Implementacao sequencial do algoritmo de Monte Carlo para calculo de casas do pi	*
*												*
*	Alunos:											*
*		Felipe Augusto do Amaral (7239221)						*
*		Julio Cesar Gaban (7152872)							*
*		Tiago Santana de Nazare (7153118)						*
*												*
*	Execucao:										*
*		./<executavel> <numero_de_iteracoes>						*
*												*
*	Compilacao:										*
*		gcc <fonte> -o <executavel> -lgmp -ansi -Wall 					*
*************************************************************************************************/

#define _SVID_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define PRECISAO 6

int main(int argc, char *argv[]){
    struct timespec time1, time2; 
    double tempo1, tempo2, auxtempo, tempototal;
	unsigned long int nAcertos = 0, i, nIteracoes;
	double x, y;
	mpf_t pi;

	/* Verifica se o argumento de entrada (numero de iteracoes) foi passado */
	if(argc != 2){
		printf("Argumentos invalidos!!!\n");
		printf("Use: %s <numero_de_iteracoes>\n", argv[0]);
		exit(1);
	}
	
	nIteracoes = strtoul(argv[1], NULL, 0);
	mpf_set_default_prec(PRECISAO);
	srand48(time(NULL));

    clock_gettime(CLOCK_REALTIME, &time1);
    
	/* Faz a simulacao (gera os pontos aleatorios e verifica se eles estao ou nao na circunferencia) */
	for(i = 0; i < nIteracoes; ++i){
		x = drand48();
		y = drand48();

		if(x * x + y * y <= 1.0){
			++nAcertos;
		}
	}
	
	/* Calcula a aproximacao de pi */
	mpf_init_set_ui(pi, nAcertos);
	mpf_div_ui(pi, pi, nIteracoes);
	mpf_mul_ui(pi, pi, 4);

	printf("pi é aproximadamente: ");
	mpf_out_str(stdout, 10, 0, pi);
	putchar ('\n');

    clock_gettime(CLOCK_REALTIME, &time2); 

    tempo1 = (int)time1.tv_nsec/1000;
    auxtempo = (int)time1.tv_sec;
    tempo1 = tempo1/1000000 + auxtempo;
    
    tempo2 = (int)time2.tv_nsec/1000;
    auxtempo = (int)time2.tv_sec;
    tempo2 = tempo2/1000000 + auxtempo;
        
    tempototal = tempo2 - tempo1; 
    
    printf("\ntempo total: %lf segundos\n", tempototal);
    
	mpf_clear(pi);
	return 0;
}

