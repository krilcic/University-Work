#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m;
pthread_cond_t queue[4];

int m_index = 1;
int k_index = 1;
int coast = 1;
int left_coast[20];
int left_coast_type[20];
int n_left_coast = 0;
int right_coast[20];
int right_coast_type[20];
int n_right_coast = 0;
int boat[7];
int boat_type[7];
int n_boat = 0;
int num_cannibals = 0;
int num_missionaries = 0;
int traveling = 0;
int printing = 1;

void insert(int list, int index, int type)
{
	if (list == 0)
	{
		left_coast[n_left_coast] = index;
		left_coast_type[n_left_coast] = type;
		n_left_coast++;
	}
	else if (list == 1)
	{
		right_coast[n_right_coast] = index;
		right_coast_type[n_right_coast] = type;
		n_right_coast++;
	}
	else if (list == 2)
	{
		boat[n_boat] = index;
		boat_type[n_boat] = type;
		n_boat++;
	}
}

void remove(int list, int ozn, int type)
{
	int index;
	if (list == 0)
	{
		for (int i = 0; i < n_left_coast; i++)
		{
			if (left_coast[i] == ozn && left_coast_type[i] == type)
			{
				index = i;
			}
		}

		if (index == n_left_coast - 1)
		{
			n_left_coast--;
		}
		else
		{
			for (int j = index; j < n_left_coast - 1; j++)
			{
				left_coast[j] = left_coast[j + 1];
				left_coast_type[j] = left_coast_type[j + 1];
			}
			n_left_coast--;
		}
	}
	else if (list == 1)
	{
		for (int i = 0; i < n_right_coast; i++)
		{
			if (right_coast[i] == ozn && right_coast_type[i] == type)
			{
				index = i;
			}
		}

		if (index == n_right_coast - 1)
		{
			n_right_coast--;
		}
		else
		{
			for (int j = index; j < n_right_coast - 1; j++)
			{
				right_coast[j] = right_coast[j + 1];
				right_coast_type[j] = right_coast_type[j + 1];
			}
			n_right_coast--;
		}
	}
	else if (list == 2)
	{
		for (int i = 0; i < n_boat; i++)
		{
			if (boat[i] == ozn && boat_type[i] == type)
			{
				index = i;
			}
		}

		if (index == n_boat - 1)
		{
			n_boat--;
		}
		else
		{
			for (int j = index; j < n_boat - 1; j++)
			{
				boat[j] = boat[j + 1];
				boat_type[j] = boat_type[j + 1];
			}

			n_boat--;
		}
	}
}

void custom_print()
{
	if (coast == 1)
	{
		printf("boat[D]={");
	}
	else
	{
		printf("boat[L]={");
	}

	for (int i = 0; i < n_boat; i++)
	{
		if (i == n_boat - 1)
		{
			if (boat_type[i] == 0)
			{
				printf("M%d", boat[i]);
			}
			else
			{
				printf("C%d", boat[i]);
			}
		}
		else
		{
			if (boat_type[i] == 0)
			{
				printf("M%d ", boat[i]);
			}
			else
			{
				printf("C%d ", boat[i]);
			}
		}
	}

	printf("} left_coast={");

	for (int i = 0; i < n_left_coast; i++)
	{
		if (i == n_left_coast - 1)
		{
			if (left_coast_type[i] == 0)
			{
				printf("M%d", left_coast[i]);
			}
			else
			{
				printf("C%d", left_coast[i]);
			}
		}
		else
		{
			if (left_coast_type[i] == 0)
			{
				printf("M%d ", left_coast[i]);
			}
			else
			{
				printf("C%d ", left_coast[i]);
			}
		}
	}

	printf("} right_coast={");

	for (int i = 0; i < n_right_coast; i++)
	{
		if (i == n_right_coast - 1)
		{
			if (right_coast_type[i] == 0)
			{
				printf("M%d", right_coast[i]);
			}
			else
			{
				printf("C%d", right_coast[i]);
			}
		}
		else
		{
			if (right_coast_type[i] == 0)
			{
				printf("M%d ", right_coast[i]);
			}
			else
			{
				printf("C%d ", right_coast[i]);
			}
		}
	}

	printf("}\n");
}

void *boat_thread(void *p)
{
	printf("Boat: empty on right coast\n");
	custom_print();
	
	while (1)
	{
		pthread_mutex_lock(&m);

		printing = 1;

		while (n_boat < 3)
		{
			pthread_cond_wait(&queue[3], &m);
		}

		printf("Boat: three passengers entered, going in one second\n");

		custom_print();
		printing = 0;

		pthread_mutex_unlock(&m);
		pthread_cond_broadcast(&queue[coast]);

		sleep(1);

		pthread_mutex_lock(&m);

		traveling = 1;

		if (coast == 0)
		{
			printf("Boat: traveling from left to right coast :");
		}
		else
		{
			printf("Boat: traveling from right to left coast :");
		}

		for (int i = 0; i < n_boat; i++)
		{
			if (i == n_boat - 1)
			{
				if (boat_type[i] == 0)
				{
					printf("M%d\n", boat[i]);
				}
				else
				{
					printf("C%d\n", boat[i]);
				}
			}
			else
			{
				if (boat_type[i] == 0)
				{
					printf("M%d ", boat[i]);
				}
				else
				{
					printf("C%d ", boat[i]);
				}
			}
		}

		pthread_mutex_unlock(&m);

		for (int i = 0; i < 2; i++)
		{
			sleep(1);
		}

		pthread_mutex_lock(&m);
		if (coast == 0)
		{
			printf("Boat: arrived from left to the right coast: ");
		}
		else
		{
			printf("Boat: arrived from right to the left coast: ");
		}

		for (int i = 0; i < n_boat; i++)
		{
			if (i == n_boat - 1)
			{
				if (boat_type[i] == 0)
				{
					printf("M%d\n", boat[i]);
				}
				else
				{
					printf("C%d\n", boat[i]);
				}
			}
			else
			{
				if (boat_type[i] == 0)
				{
					printf("M%d ", boat[i]);
				}
				else
				{
					printf("C%d ", boat[i]);
				}
			}
		}

		traveling = 0;
		coast = 1 - coast;
		n_boat = 0;
		num_cannibals = 0;
		num_missionaries = 0;

		if (coast == 0)
		{
			printf("Boat: empty on left coast\n");
		}
		else
		{
			printf("Boat: empty on right coast\n");
		}

		custom_print();

		printing = 1;

		pthread_cond_broadcast(&queue[coast]);
		pthread_cond_broadcast(&queue[2]);

		pthread_mutex_unlock(&m);
	}
}

void *cannibal_thread(void *p)
{
	int my_index = *((int *)p);
	int side = rand() & 1;

	pthread_mutex_lock(&m);
	
	if (side == 0)
	{
		printf("C%d: arrived to the left coast\n", my_index);
		insert(0, my_index, 1);
	}
	else
	{
		printf("C%d: arrived to the right coast\n", my_index);
		insert(1, my_index, 1);
	}
	custom_print();

	while (coast != side || traveling == 1)
	{
		pthread_cond_wait(&queue[side], &m);
	}

	while (n_boat == 7 || traveling == 1)
	{
		pthread_cond_wait(&queue[side], &m);
	}

	if (n_boat == 3 && printing == 1)
	{
		pthread_cond_wait(&queue[side], &m);
	}

	printf("C%d: entered the boat\n", my_index);

	if (side == 0)
	{
		remove(0, my_index, 1);
	}
	else
	{
		remove(1, my_index, 1);
	}

	insert(2, my_index, 1);
	num_cannibals++;
	custom_print();

	pthread_cond_signal(&queue[3]);
	while (side == coast)
	{
		pthread_cond_wait(&queue[2], &m);
	}

	pthread_mutex_unlock(&m);
}

void *missionaries_thread(void *p)
{
	int my_index = *((int *)p);
	int side = rand() & 1;

	pthread_mutex_lock(&m);

	if (side == 0)
	{
		printf("M%d: arrived to the left coast\n", my_index);
		insert(0, my_index, 0);
	}
	else
	{
		printf("M%d: arrived to the right coast\n", my_index);
		insert(1, my_index, 0);
	}
	custom_print();

	while (coast != side || traveling == 1)
	{
		pthread_cond_wait(&queue[side], &m);
	}

	while ((num_cannibals > (num_missionaries + 1)) || n_boat == 7 || traveling == 1)
	{
		pthread_cond_wait(&queue[side], &m);
	}

	if (n_boat == 3 && printing == 1)
	{
		pthread_cond_wait(&queue[side], &m);
	}

	printf("M%d: entered the boat\n", my_index);

	if (side == 0)
	{
		remove(0, my_index, 0);
	}
	else
	{
		remove(1, my_index, 0);
	}

	insert(2, my_index, 0);
	num_missionaries++;
	custom_print();

	pthread_cond_signal(&queue[3]);

	while (side == coast)
	{
		pthread_cond_wait(&queue[2], &m);
	}

	pthread_mutex_unlock(&m);
}

void *create(void *p)
{
	pthread_t thr_id;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	while (1)
	{
		pthread_create(&thr_id, &attr, cannibal_thread, (void *)(&k_index));
		k_index++;

		sleep(1);

		pthread_create(&thr_id, &attr, missionaries_thread, (void *)(&m_index));
		m_index++;

		pthread_create(&thr_id, &attr, cannibal_thread, (void *)(&k_index));
		k_index++;

		sleep(1);
	}
}

int main()
{
	pthread_t thr_id;
	pthread_attr_t attr;

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&queue[0], NULL);
	pthread_cond_init(&queue[1], NULL);
	pthread_cond_init(&queue[2], NULL);
	pthread_cond_init(&queue[3], NULL);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	srand((unsigned int)time(NULL));

	pthread_create(&thr_id, &attr, boat_thread, NULL);
	sleep(1);
	pthread_create(&thr_id, &attr, create, NULL);

	while (1)
	{
		sleep(1);
	}

	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&queue[0]);
	pthread_cond_destroy(&queue[1]);
	pthread_cond_destroy(&queue[2]);
	pthread_cond_destroy(&queue[3]);

	return 0;
}