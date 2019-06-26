/************************************************************************************************
*			Primeiro Trabalho de Programacao Concorrente				*	
*	Implementacao sequencial do algoritmo de Gauss-Legendre para calculo de casas do pi	*
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
	unsigned long int i, nIteracoes;
	mpf_t a1, a2, b1, b2, p1, p2, t1, t2, pi;

	/* Verifica se o argumento de entrada (numero de iteracoes) foi passado */
	if(argc != 2){
		printf("Argumentos invalidos!!!\n");
		printf("Use: %s <numero_de_iteracoes>\n", argv[0]);
		exit(1);
	}

	nIteracoes = strtoul(argv[1], NULL, 0);

	mpf_set_default_prec(PRECISAO);

	/* Inicia as variaveis */
	/* a0 = 1 */
	mpf_init_set_d(a1, 1.0);

	/* b0 = 1 / 2^(0.5) */
	mpf_init_set_d(b1, 2.0);
	mpf_sqrt(b1, b1);
	mpf_div(b1, a1, b1);

	/* t0 = 1 / 4 */
	mpf_init_set_d(t1, 0.25);

	/* p0 = 1 */
	mpf_init_set_d(p1, 1.0);

	mpf_init(a2);
	mpf_init(b2);
	mpf_init(t2);
	mpf_init(p2);
	mpf_init(pi);

    clock_gettime(CLOCK_REALTIME, &time1);
    
	/* Faz as iteracoes do algoritmo */
	for(i = 0; i < nIteracoes; ++i){
		/* a2 = (a1 + b1) / 2 */
		mpf_add(a2, a1, b1);
		mpf_div_ui(a2, a2, 2);

		/* b2 = (a1 * b1)^(0.5) */
		mpf_mul(b2, a1, b1);
		mpf_sqrt(b2, b2);

		/* t2 = t1 - p1((a1 - a2)^2) */
		mpf_sub(t2, a1, a2);
		mpf_pow_ui(t2, t2, 2);
		mpf_mul(t2, p1, t2);
		mpf_sub(t2, t1, t2);

		/* p2 = 2 * p1 */
		mpf_mul_ui(p2, p1, 2);

		mpf_set(a1, a2);
		mpf_set(b1, b2);
		mpf_set(p1, p2);
		mpf_set(t1, t2);
	}

	clock_gettime(CLOCK_REALTIME, &time2); 

    tempo1 = (int)time1.tv_nsec/1000;
    auxtempo = (int)time1.tv_sec;
    tempo1 = tempo1/1000000 + auxtempo;
    
    tempo2 = (int)time2.tv_nsec/1000;
    auxtempo = (int)time2.tv_sec;
    tempo2 = tempo2/1000000 + auxtempo;
        
    tempototal = tempo2 - tempo1; 
    
    printf("\ntempo total: %lf segundos\n", tempototal);
	
	/* pi = ((a2 + b2)^2) / 4t2 */
	mpf_add(pi, a2, b2);
	mpf_pow_ui(pi, pi, 2);
	mpf_div_ui(pi, pi, 4);
	mpf_div(pi, pi, t2);

	mpf_out_str(stdout, 10, 0, pi);

	mpf_clear(a1);
	mpf_clear(a2);	
	mpf_clear(b1);
	mpf_clear(b2);
	mpf_clear(p1);
	mpf_clear(p2);
	mpf_clear(pi);

	return 0;
}
