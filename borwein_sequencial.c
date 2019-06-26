/************************************************************************************************
*			Primeiro Trabalho de Programacao Concorrente				*	
*	Implementacao sequencial do algoritmo de Borwein para calculo de casas do pi		*
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
	unsigned long int i, nIteracoes;
	mpf_t a1, a2, y1, y2, aux, aux2, pi;
    struct timespec time1, time2; 
    double tempo1, tempo2, auxtempo, tempototal;

	/* Verifica se o argumento de entrada (numero de iteracoes) foi passado */
	if(argc != 2){
		printf("Argumentos invalidos!!!\n");
		printf("Use: %s <numero_de_iteracoes>\n", argv[0]);
		exit(1);
	}

	nIteracoes = strtoul(argv[1], NULL, 0);

	mpf_set_default_prec(PRECISAO);

	/* a = 6 - 4*(2^(0.5)) */
	mpf_init_set_d(a1, 2.0);
	mpf_sqrt(a1, a1);
	mpf_mul_ui(a1, a1, 4);
	mpf_ui_sub(a1, 6, a1);

	/* y = 2^(0.5) - 1 */
	mpf_init_set_d(y1, 2.0);
	mpf_sqrt(y1, y1);
	mpf_sub_ui(y1, y1, 1);

	mpf_init(a2);
	mpf_init(y2);
	mpf_init(aux);
	mpf_init(aux2);
	mpf_init(pi);
    
    clock_gettime(CLOCK_REALTIME, &time1);

	/* Faz as iteracoes do algoritmo */
	for(i = 0; i < nIteracoes; ++i){
		/* y2 = (1 - (1 - y1^4)^(1/4)) / (1 + (1 - y1^4)^(1/4)) */
		mpf_set(aux, y1);
		mpf_pow_ui(aux, aux, 4);
		mpf_ui_sub(aux, 1, aux);
		mpf_sqrt(aux, aux);
		mpf_sqrt(aux, aux);
		mpf_ui_sub(y2, 1, aux);
		mpf_add_ui(aux, aux, 1);
		mpf_div(y2, y2, aux);
		
		/* a2 = a1(1 + y2)^4 - 2^(2k + 3)y2(1 + y2 + y2^2) */
		/* a1(1 + y2)^4 */
		mpf_set(aux, y2);
		mpf_add_ui(aux, aux, 1);
		mpf_pow_ui(aux, aux, 4);
		mpf_mul(a2, aux, a1);
		/* 2^(2k + 3)y2(1 + y2 + y2^2) */
		mpf_set_ui(aux, 2);
		mpf_pow_ui(aux, aux, 2*i + 3);
		mpf_mul(aux, aux, y2);
		mpf_set(aux2, y2);
		mpf_pow_ui(aux2, aux2, 2);
		mpf_add(aux2, aux2, y2);
		mpf_add_ui(aux2, aux2, 1);
		mpf_mul(aux, aux, aux2);
		mpf_sub(a2, a2, aux);

		mpf_set(a1, a2);
		mpf_set(y1, y2);
	}

	mpf_ui_div(pi, 1, a2);
	mpf_out_str(stdout, 10, 0, pi);
	printf("\n");

	clock_gettime(CLOCK_REALTIME, &time2); 

    tempo1 = (int)time1.tv_nsec/1000;
    auxtempo = (int)time1.tv_sec;
    tempo1 = tempo1/1000000 + auxtempo;
    
    tempo2 = (int)time2.tv_nsec/1000;
    auxtempo = (int)time2.tv_sec;
    tempo2 = tempo2/1000000 + auxtempo;
        
    tempototal = tempo2 - tempo1; 
    
    printf("\ntempo total: %lf segundos\n", tempototal);

    
    
    mpf_clear(a1);
	mpf_clear(a2);
	mpf_clear(y1);
	mpf_clear(y2);
	mpf_clear(aux);
	mpf_clear(aux2);
	mpf_clear(pi);

	return 0;
}
