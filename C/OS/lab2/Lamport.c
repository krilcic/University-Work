#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <pthread.h>

atomic_int A;
atomic_int N, M;
atomic_int *entry;
atomic_int *number;

void *Lamport(void *x)
{
	int i = *((int *)x);
	int z = M;
	do
	{
		entry[i] = 1;
		int max = number[0];
		for (int k = 0; k < N; k++)
		{
			if (number[k] > max)
			{
				max = number[k];
			}
		}

		number[i] = number[max] + 1;
		entry[i] = 0;

		for (int j = 0; j < N; j++)
		{
			while (entry[j] != 0)
				;
			while (number[j] != 0 && (number[j] < number[i] || (number[j] == number[i] && j < i)))
				;
		}

		A += 1;
		number[i] = 0;
		z -= 1;
	} while (z > 0);
}

int main(int argc, char *argv[])
{
	N = atoi(argv[1]);
	M = atoi(argv[2]);

	pthread_t thr_id[N];
	int i;
	int j;

	A = 0;
	entry = malloc(N * sizeof(atomic_int));
	number = malloc(N * sizeof(atomic_int));

	int list[N];
	for (int k = 0; k < N; k++)
	{
		list[k] = k;
	}

	for (i = 0; i < N; i++)
	{
		if (pthread_create(&thr_id[list[i]], NULL, Lamport, &list[i]) != 0)
		{
			printf("Error while creating a thread!\n");
			exit(1);
		}
	}

	for (j = 0; j < N; j++)
	{
		pthread_join(thr_id[j], NULL);
	}

	printf("A: %d\n", A);

	free(entry);
	free(number);

	return 0;
}