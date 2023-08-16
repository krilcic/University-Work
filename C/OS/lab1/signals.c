#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

void process_sigint();
void process_sigterm();
void process_sigusr1();

struct timespec t0;

void set_start_time()
{
	clock_gettime(CLOCK_REALTIME, &t0);
}

void time(void)
{
	struct timespec t;

	clock_gettime(CLOCK_REALTIME, &t);

	t.tv_sec -= t0.tv_sec;
	t.tv_nsec -= t0.tv_nsec;
	if (t.tv_nsec < 0)
	{
		t.tv_nsec += 1000000000;
		t.tv_sec--;
	}

	printf("%03ld.%03ld:\t", t.tv_sec, t.tv_nsec / 1000000);
}

#define PRINTF(format, ...)            \
	do                                 \
	{                                  \
		time();                     \
		printf(format, ##__VA_ARGS__); \
	} while (0)

void spavaj(time_t seconds)
{
	struct timespec how_much;
	how_much.tv_sec = seconds;
	how_much.tv_nsec = 0;

	while (nanosleep(&how_much, &how_much) == -1 && errno == EINTR)
		PRINTF("Was interrupted, continuing\n");
}

int kz1 = 0;
int kz2 = 0;
int kz3 = 0;
int tp = 0;

int stack[3];
int top = -1;

int isempty()
{
	if (top == -1)
		return 1;
	else
		return 0;
}

void push(int broj)
{
	top += 1;
	stack[top] = broj;
}

int pop()
{
	int x = stack[top];
	top -= 1;
	return x;
}

void printstack()
{
	int a = top;
	while (a != -1)
	{
		if (a == 0)
		{
			printf("%d, reg[%d]", stack[a], stack[a]);
		}
		else
		{
			printf("%d, reg[%d]; ", stack[a], stack[a]);
		}
		a--;
	}
	printf("\n");
}

int main()
{
	set_start_time();
	struct sigaction act;

	act.sa_handler = process_sigint;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);

	act.sa_handler = process_sigusr1;
	sigemptyset(&act.sa_mask);
	sigaction(SIGUSR1, &act, NULL);

	act.sa_handler = process_sigterm;
	sigemptyset(&act.sa_mask);
	sigaction(SIGTERM, &act, NULL);

	PRINTF("Program with PID=%ld started working\n", (long)getpid());
	PRINTF("K_Z=000, T_P=0, stack: -\n");

	while (1)
	{
		sleep(1);
	}

	return 0;
}

void process_sigint()
{
	int i;
	if (tp == 0)
	{
		kz3 = 1;
		PRINTF("ASSEMBLY: Interruption level 3 happened and it is forwarded to the processor\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: -\n", kz1, kz2, kz3, tp);
		kz3 = 0;
		push(0);
		tp = 3;
		PRINTF("Processing of interruption level 3 has started\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}
	else if (tp == 1 || tp == 2)
	{
		kz3 = 1;
		PRINTF("ASSEMBLY: Interruption level 3 happened and it is forwarded to the processor\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
		kz3 = 0;
		if (tp == 1)
		{
			push(1);
		}
		else if (tp == 2)
		{
			push(2);
		}
		tp = 3;
		PRINTF("Processing of interruption level 3 has started\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}

	for (i = 1; i <= 10; i++)
	{
		sleep(1);
	}

	PRINTF("Processing of interruption level 3 has ended\n");
	int b = pop();
	tp = b;

	if (b == 0)
	{
		PRINTF("Continuing to execute the main program\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: -\n", kz1, kz2, kz3, tp);
		if (kz2 == 1)
		{
			PRINTF("ASSEMBLY: T_P has changed, forwarding the interruption level 2 to the processor\n");
			process_sigusr1();
		}
		else if (kz1 = 1)
		{
			PRINTF("ASSEMBLY: T_P has changed, forwarding the interruption level 1 to the processor\n");
			process_sigterm();
		}
	}
	else if (b == 1)
	{
		if (kz2 == 1)
		{
			PRINTF("Continuing to execute interruption level %d\n", b);
			PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
			printstack();
			push(1);
			process_sigusr1();
		}
		else
		{
			PRINTF("Continuing to execute interruption level %d\n", b);
			PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
			printstack();
		}
	}
	else if (b == 2)
	{
		PRINTF("Continuing to execute interruption level %d\n", b);
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}
}

void process_sigusr1()
{
	int i;
	if (tp == 0)
	{
		if (kz2 == 0)
		{
			kz2 = 1;
			PRINTF("ASSEMBLY: Interruption level 2 happened and it is forwarded to the processor\n");
			PRINTF("K_Z=%d%d%d, T_P=%d, stack: -\n", kz1, kz2, kz3, tp);
		}
		kz2 = 0;
		push(0);
		tp = 2;
		PRINTF("Processing of interruption level 2 has started\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}
	else if (tp == 1 && kz2 == 0)
	{
		kz2 = 1;
		PRINTF("ASSEMBLY: Interruption level 2 happened and it is forwarded to the processor\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
		kz2 = 0;
		push(1);
		tp = 2;
		PRINTF("Processing of interruption level 2 has started\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}
	else if (tp == 1 && kz2 == 1)
	{
		PRINTF("ASSEMBLY: T_P has changed, forwarding the interruption level 2 to the processor\n");
		PRINTF("Processing of interruption level 2 has started\n");
		kz2 = 0;
		tp = 2;
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}
	else if (tp == 3)
	{
		kz2 = 1;
		PRINTF("ASSEMBLY: Interruption level 2 happened but it is remembered and not forwarded to the processor\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}

	if (tp != 3)
	{
		for (i = 1; i <= 10; i++)
		{
			sleep(1);
		}
		PRINTF("Processing of interruption level 2 has ended\n");
		int b = pop();
		tp = b;

		if (b == 0)
		{
			PRINTF("Continuing to execute the main program\n");
			PRINTF("K_Z=%d%d%d, T_P=%d, stack: -\n", kz1, kz2, kz3, tp);
			if (kz1 = 1)
			{
				PRINTF("ASSEMBLY: T_P has changed, forwarding the interruption level 1 to the processor\n");
				process_sigterm();
			}
		}
		else
		{
			PRINTF("Continuing to execute interruption level %d\n", b);
			PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
			printstack();
		}
	}
}

void process_sigterm()
{
	int i;
	if (tp == 0)
	{
		if (kz1 == 0)
		{
			kz1 = 1;
			PRINTF("ASSEMBLY: Interruption level 1 happened and it is forwarded to the processor\n");
			PRINTF("K_Z=%d%d%d, T_P=%d, stack: -\n", kz1, kz2, kz3, tp);
		}
		kz1 = 0;
		push(0);
		tp = 1;
		PRINTF("Processing of interruption level 1 has started\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}
	else if (tp == 2 || tp == 3)
	{
		kz1 = 1;
		PRINTF("ASSEMBLY: Interruption level 1 happened but it is remembered and not forwarded to the processor\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: ", kz1, kz2, kz3, tp);
		printstack();
	}

	if (tp != 3 && tp != 2)
	{
		for (i = 1; i <= 10; i++)
		{
			sleep(1);
		}
		PRINTF("Processing of interruption level 1 has ended\n");
		int b = pop();
		tp = b;
		PRINTF("Continuing to execute the main program\n");
		PRINTF("K_Z=%d%d%d, T_P=%d, stack: -\n", kz1, kz2, kz3, tp);
	}
}