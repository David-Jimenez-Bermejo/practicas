#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

#define	SIZE	60000
#define SEED	479823554

int main(int nargs, char *argv[]) {
	clock_t start, end;
	double cpu_time_used;
	double tiempoTot, tiempoPar;
	tiempoTot = omp_get_wtime();
	start = clock();
	long i,j;
	long check;

	/* 1. No cambiar: Inicialición de arrays con números aleatorios */
	long *array = (long *)malloc( sizeof(long) * (size_t)SIZE );
	long *array2 = (long *)malloc( sizeof(long) * (size_t)SIZE );
	if ( array == NULL || array2 == NULL ) {
		fprintf( stderr, "Error allocating array for %d elements\n", SIZE );
		exit( EXIT_FAILURE );
	}
	srand48( SEED );
	for( i=0; i<SIZE; i++ ) {
		array[i] = (long)(SIZE * drand48() );
		array2[i] = (long)(SIZE * drand48() );
	}

	/* 2. Paralelizar los siguientes grupos de bucles */

	tiempoPar = omp_get_wtime();
	#pragma omp parallel for ordered schedule (static, 2)
	for( i=0; i<SIZE; i++ ) {
		#pragma omp ordered
		for( j=0; j<(i+1); j++ ) {
			array[i] = ( array[i] + array2[i] + 1
					+ (array[i]+array2[i])/2 ) % SIZE;
		}
	}

	check = 0;
	for( i=0; i<SIZE; i++ ) {
		check += array[i];
	}

	tiempoPar = omp_get_wtime() - tiempoPar;
	tiempoTot = omp_get_wtime() - tiempoTot;
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("Tiempo transcurrido: %f segundos\n", cpu_time_used);
	printf("Tiempo total: %lf, Paralelo: %lf\n", tiempoTot, tiempoPar);

	/* 3. Escribir resultados */
	printf("Result: %ld\n", check );
	return 0;
}

