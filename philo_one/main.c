#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

# define REFLESH_TIME_INTERVAL 3600000000

static long long	number_of_philosophers = 4;
static long long	time_to_die = 500;
static long long	time_to_eat = 20;
static long long	time_to_sleep = 10;
pthread_mutex_t		mutex_test[4];
long long			last_meal[4];
long long			current_time[4];

void		*eatNsleep(void *num)
{
	long long		my_number;
	long long		left;
	long long		right;
 
	my_number = &(long long *)(num);
	left = (!my_number) ? number_of_philosophers : my_number - 1;
	right = (my_number + 1 >= number_of_philosophers) ? 0 : my_number + 1;
	while (current_time[my_number] - last_meal[my_number] < time_to_die)
	{
		pthread_mutex_lock(&(mutex_test[left]));
		printf("%lld is waiting\n", my_number);
		pthread_mutex_lock(&(mutex_test[right]));
		printf("%lld start eating\n", my_number);
		usleep(time_to_eat);
		last_meal[my_number] = current_time[my_number];
		pthread_mutex_unlock(&(mutex_test[left]));
		pthread_mutex_unlock(&(mutex_test[right]));

		printf("%lld sleep\n", my_number);
		usleep(time_to_sleep);
		printf("%lld thinking\n", my_number);
	}
	return (num);
}

void		*philosopher(void *num)
{
	struct timeval	mytime;
	pthread_t		hand;
	long long		my_number;

	my_number = &(long long *)(num);
	gettimeofday(&mytime, NULL);
	current_time[my_number] = mytime.tv_usec;
	last_mea[my_number] = current_time[my_number];
	pthread_create(&hand, NULL, eatNsleep, num);
	pthread_detach(hand);
	while (current_time[my_number] - last_meal[my_number] < time_to_die)
	{
		gettimeofday(&mytime, NULL);
		if (mytime.tv_usec > REFLESH_TIME_INTERVAL)
		{
			mytime.tv_usec -= REFLESH_TIME_INTERVAL;
			settimeofday(&mytime, NULL);
			last_meal[my_number] -= REFLESH_TIME_INTERVAL;
		}
		current_time[my_number] = mytime.tv_usec;
	}
	printf("%d died\n", my_number);
	return (num);
}

int			main(void)
{
	pthread_t	ppl[number_of_philosoplers];
	long long	i;

	i = 0;
	while (i++ < number_of_philosophers)
	{
		pthread_create(&(ppl[i]), NULL, philosopher, i);
	}

	return (0);
}
