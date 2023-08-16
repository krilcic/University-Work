#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdatomic.h>

int id;
atomic_int *A;
atomic_int *rights;
atomic_int *flagi;
atomic_int *flagj;

void process1(int M)
{
	atomic_int i = M;
	do
	{
		*flagi = 1;
		while (*flagj != 0)
		{
			if (*rights == 1)
			{
				*flagi = 0;
				while (*rights == 1);
				*flagi = 1;
			}
		}

		*A += 1;
		*rights = 1;
		*flagi = 0;
		i -= 1;
	} while (i > 0);
}

void process2(int M)
{
	atomic_int j = M;
	do
	{
		*flagj = 1;
		while (*flagi != 0)
		{
			if (*rights == 0)
			{
				*flagj = 0;
				while (*rights == 0);
				*flagj = 1;
			}
		}

		*A += 1;
		*rights = 0;
		*flagj = 0;
		j -= 1;
	} while (j > 0);
}

void delete(int sig)
{
	(void)shmdt((char *)A);
	(void)shmdt((char *)rights);
	(void)shmdt((char *)flagi);
	(void)shmdt((char *)flagj);
	(void)shmctl(id, IPC_RMID, NULL);
	exit(0);
}

int main(int argc, char *argv[])
{
	int M = atoi(argv[1]);
	id = shmget(IPC_PRIVATE, 4 * sizeof(int), 0600);
	if (id == -1)
	{
		exit(1);
	}

	A = (atomic_int *)shmat(id, NULL, 0);
	rights = (atomic_int *)shmat(id, NULL, 0) + 1;
	flagi = (atomic_int *)shmat(id, NULL, 0) + 2;
	flagj = (atomic_int *)shmat(id, NULL, 0) + 3;
	*A = 0;

	struct sigaction act;
	act.sa_handler = delete;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);

	if (fork() == 0)
	{
		process1(M);
		exit(0);
	}

	if (fork() == 0)
	{
		process2(M);
		exit(0);
	}

	(void)wait(NULL);
	(void)wait(NULL);

	printf("A: %d\n", *A);
	delete (0);

	return 0;
}