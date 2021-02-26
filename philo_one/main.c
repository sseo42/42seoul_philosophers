#include "philosopher.h"
#include "philosopher_util.h"

void		fillup_phil(t_arg arg, t_mtx *mtx, t_phil *phil)
{
	long int idx;
	
	idx = 0;
	while (idx < arg.number_of_phil)
	{
		phil[idx].my_number = idx + 1;
		phil[idx].time_to_die = arg.time_to_die;
		phil[idx].time_to_eat = arg.time_to_eat;
		phil[idx].time_to_sleep = arg.time_to_sleep;
		phil[idx].must_eat = arg.must_eat;
		if (!idx)
			phil[idx].left = &(mtx[arg.number_of_phil - 1].stick);
		else
			phil[idx].left = &(mtx[idx - 1].stick);
		phil[idx].right = &(mtx[idx].stick);
		phil[idx].last_meal = 0;
		idx++;
	}
}

int			launch_philosophers(t_arg arg, t_mtx *mtx, t_phil *phil)
{
	long int idx;
	
	idx = 0;
	fillup_phil(arg, mtx, phil);
	while (idx < arg.number_of_phil)
	{
		if (pthread_create(&(mtx[idx].philosopher), NULL, doPhilosopher, (void *)(phil + idx)))
			return (3);
		idx += 2;
	}
	idx = 1;
	while (idx < arg.number_of_phil)
	{
		if (pthread_create(&(mtx[idx].philosopher), NULL, doPhilosopher, (void *)(phil + idx)))
			return (3);
		idx += 2;
	}
	idx = 0;
	while (idx < arg.number_of_phil)
	{
		if (pthread_join(mtx[idx].philosopher, NULL))
			return (3);
		idx++;
	}
	free(phil);
	free(mtx);
	return (0);
}

t_arg		extract_arg(char **argv, int option_flag)
{
	t_arg	arg;

	arg.number_of_phil = check_digit(argv[1]);
	arg.time_to_die = check_digit(argv[2]) * 1000;
	arg.time_to_eat = check_digit(argv[3]) * 1000;
	arg.time_to_sleep = check_digit(argv[4]) * 1000;
	if (arg.time_to_die < 0 || arg.time_to_eat < 0 || arg.time_to_sleep < 0)
		arg.number_of_phil = -1;
	if (option_flag)
	{
		arg.must_eat = check_digit(argv[5]);
		if (arg.must_eat < 0)
			arg.number_of_phil = -1;
	}
	else
		arg.must_eat = -1;
	return (arg);
}

t_mtx		*init_mtx(long int number_of_phil)
{
	t_mtx	*mtx;
	long int idx;

	if (!(mtx = (t_mtx *)malloc(sizeof(t_mtx) * number_of_phil)))
		return (0);
	idx = 0;
	while (idx < number_of_phil)
	{
		pthread_mutex_init(&(mtx[idx].stick), NULL);
		idx++;
	}
	return (mtx);
}

int			main(int argc, char **argv)
{
	t_arg			 	arg;
	t_mtx				*mtx;
	t_phil				*phil;

	if (argc != 5 && argc != 6)
		return (1); //wrong number of argments
	arg = extract_arg(argv, argc == 6);
	if (arg.number_of_phil < 2)
		return (1); //wrong argments
	if (!(mtx = init_mtx(arg.number_of_phil)))
		return (2); //malloc fail
	if (!(phil = (t_phil *)malloc(sizeof(t_phil) * arg.number_of_phil)))
	{
		free(mtx);
		return (2); //malloc fail
	}
	if ((launch_philosophers(arg, mtx, phil)))
	{
		free(mtx);
		free(phil);
		return (3); //pthread fail
	}
	return (0);
}