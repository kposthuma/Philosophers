/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/20 17:40:29 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/04 11:38:06 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

// void	f(void)
// {
// 	system("leaks philo");
// }
	// atexit(f);

int	main(int argc, char **argv)
{
	t_philos	*philos;
	pthread_t	*thread;
	pthread_t	thread2;
	size_t		x;

	if (check_input(argc, argv) != 0)
		return (1);
	philos = init_philos(argv);
	thread = malloc((arg_to_int(argv[1]) + 1) * sizeof(pthread_t));
	if (!thread || !philos)
		return (philo_error(NULL), free(thread), 1);
	pthread_mutex_init(&philos->lock, NULL);
	x = 0;
	while (x < philos->number_of_philos)
		pthread_create(&thread[x++], NULL, &philo_thread, (void *)philos);
	pthread_create(&thread[x], NULL, &is_dead, (void *)philos);
	if (philos->number_of_meals != 0)
		pthread_create(&thread2, NULL, &has_eaten, (void *)philos);
	x = 0;
	while (x < (philos->number_of_philos + 1))
		pthread_join(thread[x++], NULL);
	if (philos->number_of_meals != 0)
		pthread_join(thread2, NULL);
	pthread_mutex_destroy(&philos->lock);
	return (philo_liberation(philos), free(thread), 0);
}

void	philo_copy_data(t_thinker *thinker, t_philos *philos)
{
	thinker->number_of_philos = philos->number_of_philos;
	thinker->time_to_die = philos->time_to_die;
	thinker->time_to_eat = philos->time_to_eat;
	thinker->time_to_sleep = philos->time_to_sleep;
	thinker->number_of_meals = philos->number_of_meals;
	thinker->start_time = philos->start_time;
}

t_thinker	**make_philos(size_t num, t_philos *philos)
{
	t_thinker	**thinker;
	size_t		i;

	thinker = malloc(sizeof(t_thinker *) * num);
	if (!thinker)
		return (NULL);
	i = 0;
	while (i < num)
	{
		thinker[i] = malloc(sizeof(t_thinker));
		if (!thinker[i])
			return (free_thinkers(thinker, i), NULL);
		thinker[i]->philo_id = i + 1;
		thinker[i]->fork = 0;
		thinker[i]->life = true;
		thinker[i]->last_supper = get_time();
		thinker[i]->meals_eaten = 0;
		thinker[i]->finished = false;
		philo_copy_data(thinker[i], philos);
		i++;
	}
	return (thinker);
}

t_philos	*init_philos(char **argv)
{
	t_philos	*philos;

	philos = malloc(sizeof(t_philos));
	if (!philos)
		return (NULL);
	philos->number_of_philos = arg_to_int(argv[1]);
	philos->time_to_die = arg_to_int(argv[2]);
	philos->time_to_eat = arg_to_int(argv[3]);
	philos->time_to_sleep = arg_to_int(argv[4]);
	philos->number_of_meals = arg_to_int(argv[5]);
	philos->start_time = get_time();
	philos->thinker = make_philos(philos->number_of_philos, philos);
	if (!philos->thinker)
		return (free(philos), NULL);
	return (philos);
}

int	check_input(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (philo_error("Incorrect number of arguments.\n\
Please input either four or five arguments.\n"), 1);
	if (arg_to_int(argv[1]) < 1 || arg_to_int(argv[2]) < 1 || arg_to_int(argv[3]
		) < 1 || arg_to_int(argv[4]) < 1 || arg_to_int(argv[5]) < 0)
		return (philo_error("Invalid argument value.\n\
Please input only posisitve integers.\n"), 1);
	return (0);
}
