/************************************************************************************************
*			Primeiro Trabalho de Programacao Concorrente				*	
*	Implementacao paralela do algoritmo de Monte Carlo para calculo de casas do pi		*
*												*
*	Alunos:											*
*		Felipe Augusto do Amaral (7239221)						*
*		Julio Cesar Gaban (7152872)							*
*		Tiago Santana de Nazare (7153118)						*
*												*
*	Execucao:										*
*		./<executavel> <numero_de_iteracoes> <numero_de_threads>			*
*												*
*	Compilacao:										*
*		gcc <fonte> -o <executavel> -lgmp -ansi -Wall -pthread				*
*************************************************************************************************/

#define _SVID_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include <pthread.h>

#define PRECISAO 6

/* Parametros passados as threads */
struct thread_params_t{
	unsigned long int acertos; /* Numero de acertos (parametro de retorno)  */
	unsigned long int iteracoes; /* Numero de iteracoes que devem ser realidas pela thread */
};
typedef struct thread_params_t thread_params_t;

void *thread_monte_carlo(void *param);

int main(int argc, char *argv[]){
    struct timespec time1, time2; 
    double tempo1, tempo2, auxtempo, tempototal;
	pthread_t *threads;
	thread_params_t *threadParams;
	int i, nThreads;
	unsigned long int nIteracoes, nIteracoesThread; 
	mpf_t pi;
	
	/* Verifica se os argumentos de entrada foram passados */
	if(argc != 3){
		printf("Argumentos invalidos!!!\n");
		printf("Use: %s <numero_de_iteracoes> <numero_de_threads>\n", argv[0]);
		exit(1);
	}

	nIteracoes = strtoul(argv[1], NULL, 0);
	nThreads = (int)strtoul(argv[2], NULL, 0);
	nIteracoesThread = nIteracoes / nThreads;
	
	mpf_set_default_prec(PRECISAO);
	
	threads = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
	threadParams = (thread_params_t *)malloc(sizeof(thread_params_t) * nThreads);	

    clock_gettime(CLOCK_REALTIME, &time1);

	/* Cria as threads que farao as simulacoes */
	for (i = 0; i < nThreads; ++i){
		threadParams[i].iteracoes = nIteracoesThread;
		pthread_create(&threads[i], NULL, thread_monte_carlo, (void *)&threadParams[i]);
	}

	/* Calcula a aproximacao de pi */
	mpf_init_set_ui(pi, 0);
	for (i = 0; i < nThreads; ++i){
		pthread_join(threads[i], NULL);
		mpf_add_ui(pi, pi, threadParams[i].acertos);
	}
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
    
	free(threads);
	free(threadParams);
	mpf_clear(pi);
	return 0;
}

/* Funcao para fazer as simulacoes (usada pelas threads) */
void *thread_monte_carlo(void *param){
	thread_params_t *threadParams = (thread_params_t *)param;
	unsigned long int nAcertosAux = 0, i, nIteracoes = threadParams->iteracoes;
	double x, y;
	struct drand48_data drand_buffer;

	/* Faz a simulacao (gera os pontos aleatorios e verifica se eles estao ou nao na circunferencia) */
	srand48_r(time(NULL), &drand_buffer);
	for(i = 0; i < nIteracoes; ++i){
		drand48_r(&drand_buffer, &x);
		drand48_r(&drand_buffer, &y);

		if(x * x + y * y <= 1.0){
			++nAcertosAux;
		}
	}

	threadParams->acertos = nAcertosAux;

	pthread_exit(NULL);
}

