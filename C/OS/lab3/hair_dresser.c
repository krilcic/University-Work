#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

sem_t *working;
sem_t *q;
sem_t *arrived;
int *open;
int *N;
int *client_number;
int *index;
int (*queue)[5];
int *cnt;

void insert(int client_number)
{
	if (*index < 5)
	{
		(*queue)[(*index)] = client_number;
		(*index)++;
	}
}

int take()
{
	int number;
	number = (*queue)[0];

	for (int i = 0; i < (*index); i++)
	{
		(*queue)[i] = (*queue)[i + 1];
	}
	(*queue)[(*index)] = -1;

	if ((*index) > 0)
	{
		(*index)--;
	}

	return number;
}

void hairdresser()
{
	printf("Hairdresser: Openning salon\n");
	printf("Hairdresser: Putting the open sign\n");

	while (1)
	{
		if (*open == 0 && *cnt == 0)
		{
			printf("Hairdresser: Putting the close sign\n");
			(*cnt)++;
		}

		if (*N > 0)
		{
			sem_wait(arrived);
			(*N)--;
			sem_post(arrived);
			int current_client;
			sem_wait(q);
			current_client = take();
			sem_post(q);
			printf("Hairdresser: Starting to work on client %d\n", current_client);
			printf("Client(%d): Hairdresser is cutting my hair\n", current_client);
			for (int i = 0; i < 5; i++)
			{
				sleep(1);
			}
			printf("Hairdresser: Client %d done\n", current_client);
			sem_wait(working);
		}
		else if (*open == 1)
		{
			printf("Hairdresser: Sleeping while there are no clients\n");
			sem_wait(working);
			sem_post(working);
		}
		else
		{
			printf("Hairdresser: Closing salon\n");
			break;
		}
	}
}

void client(int client_number)
{
	printf("Client(%d): I want a haircut\n", client_number);
	if (*open == 1 && *N < 3)
	{
		sem_wait(arrived);
		(*N)++;
		printf("Client(%d): Entering the waiting room (%d)\n", client_number, *N);
		sem_wait(q);
		insert(client_number);
		sem_post(q);
		sem_post(working);
		sem_post(arrived);
	}
	else
	{
		printf("Client(%d): I will come back tomorrow\n", client_number);
	}
}

int main()
{
	int ID = shmget(IPC_PRIVATE, 3 * sizeof(sem_t) + 5 * sizeof(int) + sizeof(int[5]), 0600);
	void *p = shmat(ID, NULL, 0);
	shmctl(ID, IPC_RMID, NULL);

	working = (sem_t *)p;
	q = working + 1;
	arrived = q + 1;
	open = (int *)(arrived + 1);
	N = open + 1;
	client_number = N + 1;
	index = client_number + 1;
	cnt = index + 1;
	queue = (int(*)[5])(cnt + 1);

	sem_init(working, 1, 0);
	sem_init(q, 1, 1);
	sem_init(arrived, 1, 1);

	*open = 1;
	*N = 0;
	*client_number = 1;
	*index = 0;
	*cnt = 0;

	if (fork() == 0)
	{
		hairdresser();
		exit(0);
	}

	for (int i = 0; i < 5; i++)
	{
		if (fork() == 0)
		{
			client(*client_number);
			exit(0);
		}
		else
		{
			sleep(1);
			(*client_number)++;
		}
	}

	for (int i = 0; i < 20; i++)
	{
		sleep(1);
	}

	if (fork() == 0)
	{
		client(*client_number);
		exit(0);
	}
	else
	{
		sleep(1);
		(*client_number)++;
	}

	for (int i = 0; i < 10; i++)
	{
		sleep(1);
	}

	if (fork() == 0)
	{
		client(*client_number);
		exit(0);
	}
	else
	{
		sleep(1);
		(*client_number)++;
	}

	for (int i = 0; i < 10; i++)
	{
		sleep(1);
	}

	*open = 0;
	sem_post(working);

	for (int i = 0; i < 8; i++)
	{
		wait(NULL);
	}

	sem_destroy(working);
	shmdt(working);
	sem_destroy(q);
	shmdt(q);
	sem_destroy(arrived);
	shmdt(arrived);
	shmdt(open);
	shmdt(N);
	shmdt(client_number);
	shmdt(index);
	shmdt(cnt);

	return 0;
}