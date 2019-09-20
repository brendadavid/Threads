#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 1000
#define NUM_THREADS 10

// ESTRUTURA QUE A THREAD MANIPULA DA MATRIZ
typedef struct{
	int inicio;
	int fim;
	int **mt1;
	int **mt2;
	int **mtr;
} THREADMATRIZ;

// DEFINICAO DA THREAD
void *thread(void *p) {
    THREADMATRIZ *m = (THREADMATRIZ *)p;
	for (int i = m->inicio; i < m->fim; i++){
		for (int j = 0; j < SIZE; j++) {
			m->mtr[i][j] = 0;
			for (int k = 0; k < SIZE; k++){
			    m->mtr[i][j] += m->mt1[i][k] * m->mt2[k][j];
			}
			printf("\n======>mres[%d][%d]: %d", i, j, m->mtr[i][j]);
		}
	}
    return NULL;
}

// PROTOTIPOS
void TimeInit (void);
double TimeStart (void);
double TimeStop (double);

// VALOR DO OVERHEAD DA MEDICAO DE TEMPO
static double TimeOverhead = 0.0;

// ESTRUTURA DE DADOS COMPARTILHADA
int **m1;
int **m2;
int **mres;
int l1, c1, l2, c2, lres, cres;

// FUNCAO QUE CALCULA O OVERHEAD DA MEDICAO DE TEMPO
void TimeInit () {
	double t;
	TimeOverhead = 0.0;
	t = TimeStart ();
	TimeOverhead = TimeStop (t);
}

// FUNCAO QUE CAPTURA O TEMPO INICIAL DO TRECHO A SER MEDIDO
double TimeStart () {
	struct timeval tv;
	struct timezone tz;

	if (gettimeofday (&tv, &tz) != 0)
		exit (1);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// FUNCAO QUE CALCULA O TEMPO GASTO NO FINAL DO TRECHO A SER MEDIDO
double TimeStop (double TimeInitial) {
	struct timeval tv;
	struct timezone tz;
	double Time;
	
	if (gettimeofday (&tv, &tz) != 0)
		exit (1);
	Time = tv.tv_sec + tv.tv_usec / 1000000.0;
	return Time - TimeInitial - TimeOverhead;
}

// FUNCAO QUE ALOCA MEMORIA PARA MATRIZ 
int **alocarMatriz()
{
	int i;
	int *val, **tmp;
	// aloca a memória
	val = (int *)malloc(SIZE * SIZE * sizeof(int));
	tmp = (int **)malloc(SIZE * sizeof(int *));
	// cria a estrutura da matriz
	for (i = 0; i < SIZE; i++) {
		tmp[i] = &(val[i * SIZE]);
	}
	return tmp;
}

int main () {
	int i, j, k;
	double inicio, total;
	
	// INICIALIZA OS ARRAYS A SEREM MULTIPLICADOS
	l1 = c1 = SIZE;
	l2 = c2 = SIZE;
	if (c1 != l2) {
		fprintf (stderr,"Impossivel multiplicar matrizes: parametros invalidos.\n");
		return 1;
	}

	// ALOCA DINAMICAMENTE A MEMORIA ATRAVES DA FUNCAO
	m1 = alocarMatriz();
	m2 = alocarMatriz();
	mres = alocarMatriz();
	
	k = 1;
	//printf ("POPULA MATRIZ 1 \n");
	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			if (k % 2 == 0) {
				m1[i][j] = -k;
			} else {
				m1[i][j] = k;
			}
		}
		k++;
	}
	
	k = 1;
	//printf ("POPULA MATRIZ 2 \n");
	for (j = 0; j < SIZE; j++) {
		for (i = 0; i < SIZE; i++) {
			if (k % 2 == 0) {
				m2[i][j] = -k;
			}
			else {
				m2[i][j] = k;
			}
		}
		k++;
	}
	
	// PREPARA ESTRUTURA DA THREAD
	THREADMATRIZ mt[NUM_THREADS];
	for (i = 0; i < NUM_THREADS; i++){
		mt[i].inicio = (SIZE / NUM_THREADS) * i;
		mt[i].fim = (SIZE / NUM_THREADS) * (i + 1) - 1;
		mt[i].mt1 = m1;
		mt[i].mt2 = m2;
		mt[i].mtr = mres;
	}
	
	pthread_t t[NUM_THREADS];
	
	// PREPARA PARA MEDIR TEMPO
	TimeInit ();
	inicio = TimeStart ();
	
	printf("Criando threads...\n");
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&t[i], NULL, thread, (void *)&mt[i]) != 0) {
            fprintf(stderr, "ERRO: pthread_create() falhou...\n");
            exit(1);
        }
    }
    printf("Vai esperar pelo final das threads...\n");
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(t[i], NULL);
    }

	// OBTEM O TEMPO
	total = TimeStop (inicio);
	
	// VERIFICA SE O RESULTADO DA MULTIPLICACAO ESTA CORRETO
	for (i = 0; i < SIZE; i++){
		k = SIZE * (i + 1);
		for (j = 0; j < SIZE; j++){
			int k_col = k * (j + 1);
			if (i % 2 == 0){
				if (j % 2 == 0){
					if (mres[i][j] != k_col){
						printf ("1 Falho %d", mt[0].mtr[i][j]);
						return 1;
					}
				} else {
					if (mres[i][j] != -k_col){
					printf ("2 Falho %d", mres[i][j]);
					return 1;
					}
				}
			} else {
				if (j % 2 == 0){
					if (mres[i][j] != -k_col){
						printf ("3 Falho %d", mres[i][j]);
						return 1;
					}
				} else {
					if (mres[i][j] != k_col){
						printf ("4 Falho %d", mres[i][j]);
						return 1;
					}
				}
			}
		}
	}

	// MOSTRA O TEMPO DE EXECUCAO
	printf ("%lf", total);
	return 0;
}
