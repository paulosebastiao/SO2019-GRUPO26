/************************************************************************************************
*			Primeiro Trabalho de Programacao Concorrente				*	
*	Implementacao paralela do algoritmo de Borwein para calculo de casas do pi		*
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
*		gcc <fonte> -o <executavel> -lgmp -pthread -ansi -Wall				*
*************************************************************************************************/


#define _SVID_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include <pthread.h>




#define PRECISAO 6

/* Parametros para o calculo da primeira parte de ak */
struct calc_p1_t{
	mpf_t *a1;
	mpf_t *y2;
	mpf_t *p1;
};
typedef struct calc_p1_t calc_p1_t;

/* Parametros para o calculo da segunda parte de ak */
struct calc_p2_t{
	unsigned long int *i;
	mpf_t *y2;
	mpf_t *p2;
};
typedef struct calc_p2_t calc_p2_t;

/* Parametros para o calculo da tercera parte de ak */
struct calc_p3_t{
	mpf_t *y2;
	mpf_t *p3;
};
typedef struct calc_p3_t calc_p3_t;

void *calc_p1(void *param);
void *calc_p2(void *param);
void *calc_p3(void *param);

int main(int argc, char *argv[]){
	unsigned long int i, nIteracoes;
	mpf_t a1, a2, y1, y2, p1, p2, p3, pi, aux;
	pthread_t thread_calc_p1, thread_calc_p2, thread_calc_p3;
	calc_p1_t data_calc_p1;
	calc_p2_t data_calc_p2;
	calc_p3_t data_calc_p3;
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
	mpf_init(p1);
	mpf_init(p2);
	mpf_init(p3);
	mpf_init(pi);
	mpf_init(aux);

	data_calc_p1.a1 = &a1;
	data_calc_p1.y2 = &y2;
	data_calc_p1.p1 = &p1;

	data_calc_p2.i = &i;
	data_calc_p2.y2 = &y2;
	data_calc_p2.p2 = &p2;

	data_calc_p3.y2 = &y2;
	data_calc_p3.p3 = &p3;
    
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
		
		pthread_create(&thread_calc_p1, NULL, calc_p1, (void *) &data_calc_p1);
		pthread_create(&thread_calc_p2, NULL, calc_p2, (void *) &data_calc_p2);
		pthread_create(&thread_calc_p3, NULL, calc_p3, (void *) &data_calc_p3);

		pthread_join(thread_calc_p1, NULL);
		pthread_join(thread_calc_p2, NULL);
		pthread_join(thread_calc_p3, NULL);

		/* a2 = a1(1 + y2)^4 - 2^(2k + 3)y2(1 + y2 + y2^2) */
		/* a2 = p1 - p2 * p3 */
		mpf_set(a2, p3);
		mpf_mul(a2, a2, p2);
		mpf_sub(a2, p1, a2);

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
	mpf_clear(p1);
	mpf_clear(p2);
	mpf_clear(p3);
	mpf_clear(pi);

	return 0;
}

/* Calcula a primeira parte de ak */
void *calc_p1(void *param){
	calc_p1_t data = *((calc_p1_t *)param);

	/* a1(1 + y2)^4 */
	mpf_set(*data.p1, *data.y2);
	mpf_add_ui(*data.p1, *data.p1, 1);
	mpf_pow_ui(*data.p1, *data.p1, 4);
	mpf_mul(*data.p1, *data.p1, *data.a1);

	pthread_exit(NULL);
}

/* Calcula a segunda parte de ak */
void *calc_p2(void *param){
	calc_p2_t data = *((calc_p2_t *)param);

	/* 2^(2k + 3)y2 */
	mpf_set_ui(*data.p2, 2);
	mpf_pow_ui(*data.p2, *data.p2, 2 * (*data.i) + 3);
	mpf_mul(*data.p2, *data.p2, *data.y2);

	pthread_exit(NULL);
}

/* Calcula a terceira parte de ak */
void *calc_p3(void *param){
	calc_p3_t data = *((calc_p3_t *)param);

	/* 2^(2k + 3)y2(1 + y2 + y2^2) */
	mpf_set(*data.p3, *data.y2);
	mpf_pow_ui(*data.p3, *data.p3, 2);
	mpf_add(*data.p3, *data.p3, *data.y2);
	mpf_add_ui(*data.p3, *data.p3, 1);

	pthread_exit(NULL);
}

