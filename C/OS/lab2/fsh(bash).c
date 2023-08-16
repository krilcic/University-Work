#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

void handle_sigint();
pid_t pid = -1;

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler = handle_sigint;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);

	char *path = getenv("PATH");
	char *path_part[10];
	int j = 0;
	path_part[j] = strtok(path, ":");
	while (path_part[j] != NULL)
	{
		j++;
		path_part[j] = strtok(NULL, ":");
	}

	while (1)
	{
		fflush(stdin);
		char line[100];
		char *arg_Ptr[10];
		printf("fsh> ");
		fgets(line, 100, stdin);

		if ((line[0] == '\n'))
		{
			break;
		}

		int i = 0;
		arg_Ptr[i] = strtok(line, " \n");

		while (arg_Ptr[i] != NULL)
		{
			i++;
			arg_Ptr[i] = strtok(NULL, " \n");
		}

		arg_Ptr[i] = NULL;

		if (strcmp(arg_Ptr[0], "exit") == 0)
		{
			break;
		}
		else if (strcmp(arg_Ptr[0], "cd") == 0)
		{
			if (arg_Ptr[1] != NULL)
			{
				if (chdir(arg_Ptr[1]) != 0)
				{
					fprintf(stderr, "cd: The directory '%s' does not exist\n", arg_Ptr[1]);
				}
			}
		}
		else
		{
			if (line[0] == '.')
			{
				printf("Wrong input!\n");
			}
			else if (line[0] == '/')
			{
				pid_t local_pid = fork();
				if (local_pid == 0)
				{
					setpgid(0, 0);
					act.sa_handler = SIG_DFL;
					sigaction(SIGINT, &act, NULL);
					sleep(2);
					execv(arg_Ptr[0], arg_Ptr);
					printf("fsh: Unknown command: '%s'\n", arg_Ptr[0]);
					exit(1);
				}
				else
				{
					pid = local_pid;
					wait(NULL);
					pid = -1;
				}
			}
			else
			{
				int pass = 0;

				for (int i = 0; i < 5; i++)
				{
					char temp[100];
					char empty[2] = "";
					strcpy(temp, empty);
					strcat(temp, path_part[i]);
					strcat(temp, "/");
					strcat(temp, arg_Ptr[0]);
					if (access(temp, F_OK) == 0)
					{
						pass = 1;
						pid_t local_pid = fork();
						if (local_pid == 0)
						{
							setpgid(0, 0);
							act.sa_handler = SIG_DFL;
							sigaction(SIGINT, &act, NULL);
							sleep(2);
							execv(temp, arg_Ptr);
							fflush(stdout);
							printf("fsh: Unknown command: '%s'\n", arg_Ptr[0]);
							exit(1);
						}
						else
						{
							pid = local_pid;
							wait(NULL);
							pid = -1;
							break;
						}
					}
				}
				if (pass == 0)
				{
					printf("fsh: Unknown command: '%s'\n", arg_Ptr[0]);
				}
			}
		}
	}

	return 0;
}

void handle_sigint()
{
	if (pid != -1)
	{
		kill(pid, SIGINT);
	}
	else
	{
		printf("\nfsh> ");
		fflush(stdout);
	}
}