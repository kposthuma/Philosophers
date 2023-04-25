/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/20 17:40:29 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 13:41:02 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

t_thinker	**make_philos(size_t num)
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
			return (NULL); //free everything
		thinker[i]->philo_id = i + 1;
		thinker[i]->fork = 0;
		thinker[i]->life = true;
		thinker[i]->last_supper = get_time();
		thinker[i]->meals_eaten = 0;
		i++;
	}
	return (thinker);
}

t_philos	*init_philos(char **argv)
{
	t_philos	*philos;

	philos = malloc(sizeof(t_philos));
	if (!philos)
		return (philo_error("Memory allocation error\n"), NULL);
	philos->number_of_philos = arg_to_int(argv[1]);
	philos->time_to_die = arg_to_int(argv[2]);
	philos->time_to_eat = arg_to_int(argv[3]);
	philos->time_to_sleep = arg_to_int(argv[4]);
	philos->number_of_meals = arg_to_int(argv[5]);
	philos->start_time = get_time();
	philos->thinker = make_philos(philos->number_of_philos);
	if (!philos->thinker)
		return (philo_error("Memory allocation error\n"), NULL);
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

// void	f(void)
// {
// 	system("leaks philo");
// }

int	main(int argc, char **argv)
{
	t_philos	*philos;
	pthread_t	*thread;
	pthread_t	thread2;
	size_t		x;

	if (check_input(argc, argv) != 0)
		return (1);
	philos = init_philos(argv);
	thread = malloc((philos->number_of_philos + 1) * sizeof(pthread_t));
	if (!thread || !philos)
		return (philo_error("Memory allocation error\n"), 1);
	pthread_mutex_init(&philos->lock, NULL);
	x = 0;
	while (x < philos->number_of_philos)
		pthread_create(&thread[x++], NULL, &philo_thread, (void *)philos);
	pthread_create(&thread[x], NULL, &is_dead, (void *)philos);
	if (philos->number_of_meals != 0)
		pthread_create(&thread2, NULL, &has_eaten, (void *)philos);
	x = 0;
	while (x < philos->number_of_philos)
		pthread_join(thread[x++], NULL);
	pthread_join(thread[x], NULL);
	pthread_mutex_destroy(&philos->lock);
	return (0);
}
