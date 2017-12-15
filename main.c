#include <stdio.h>
#include <pthread.h>

void	*routine_a(void *arg);
void	*routine_b(void *arg);

void	(*onValEqual10)(int *);

void	sample10(int * v)
{
	printf("Ok good\n");
	*v = 0;
}

int val = 0;
pthread_t t_a;
pthread_t t_b;

int main()
{

	onValEqual10 = sample10;

	if (pthread_create(&t_a, NULL, routine_a, NULL) || pthread_create(&t_b, NULL, routine_b, NULL))
	{
		printf("Failed to create phtreads\n");
		return (1);
	}

	if (pthread_join(t_a, NULL) || pthread_join(t_b, NULL))
	{
		printf("Failed to join pthreads\n");
		return (1);
	}

	return (0);
}

void	*routine_a(void *arg)
{

	while (1)
	{
		if (val == 10 && onValEqual10)
			onValEqual10(&val);
	}

	pthread_exit(NULL);
}

void	*routine_b(void *arg)
{

	while (1)
	{
		printf("\nval > ");
		scanf("%d", &val);
	}

	pthread_exit(NULL);
}
