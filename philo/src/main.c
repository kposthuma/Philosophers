/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/20 17:40:29 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/10 16:53:07 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

// void	f(void)
// {
// 	system("leaks philo");
// }

// void	fork_states(t_philos *philos)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < philos->nmb_of_philos)
// 	{
// 		if (philos->thinker[i]->fork == 0)
// 			printf("fork %lu	is not held\n", i + 1);
// 		else if (philos->thinker[i]->fork == 1)
// 			printf("fork %lu	is held by philo %lu\n", i + 1,
// 				philos->thinker[i]->philo_id);
// 		else if (philos->thinker[i]->fork == 2
// 			&& i < philos->nmb_of_philos - 1)
// 			printf("fork %lu	is held by philo %lu\n", i + 1,
// 				philos->thinker[i + 1]->philo_id);
// 		else
// 			printf("fork %lu	is held by philo %lu\n", i + 1,
// 				philos->thinker[0]->philo_id);
// 		i++;
// 	}
// }

int	main(int argc, char **argv)
{
	t_philos	*strc;
	pthread_t	*thread;
	pthread_t	thread2;
	size_t		x;

	if (check_input(argc, argv) != 0)
		return (1);
	strc = init_philos(argv);
	thread = malloc((arg_to_int(argv[1])) * sizeof(pthread_t));
	if (!thread || !strc)
		return (philo_error(NULL), free(thread), 1);
	pthread_mutex_init(&strc->lock, NULL);
	x = 0;
	while (x < strc->nmb_of_philos)
		pthread_create(&thread[x++], NULL, &philo_thread, (void *)strc);
	if (strc->nmb_of_meals != 0)
		pthread_create(&thread2, NULL, &has_eaten, (void *)strc);
	is_dead((void *)strc);
	x = 0;
	while (x < (strc->nmb_of_philos))
		pthread_join(thread[x++], NULL);
	if (strc->nmb_of_meals != 0)
		pthread_join(thread2, NULL);
	pthread_mutex_destroy(&strc->lock);
	return (philo_liberation(strc), free(thread), 0);
}

t_thinker	**make_philos(size_t num)
{
	t_thinker	**phils;
	size_t		i;

	phils = malloc(sizeof(t_thinker *) * num);
	if (!phils)
		return (NULL);
	i = 0;
	while (i < num)
	{
		phils[i] = malloc(sizeof(t_thinker));
		if (!phils[i])
			return (free_thinkers(phils, i), NULL);
		phils[i]->philo_id = i + 1;
		phils[i]->fork = 0;
		phils[i]->life = true;
		phils[i]->last_supper = get_time();
		phils[i]->meals_eaten = 0;
		phils[i]->finished = false;
		i++;
	}
	return (phils);
}

t_philos	*init_philos(char **argv)
{
	t_philos	*strc;

	strc = malloc(sizeof(t_philos));
	if (!strc)
		return (NULL);
	strc->nmb_of_philos = arg_to_int(argv[1]);
	strc->time_to_die = arg_to_int(argv[2]);
	strc->time_to_eat = arg_to_int(argv[3]);
	strc->time_to_sleep = arg_to_int(argv[4]);
	strc->nmb_of_meals = arg_to_int(argv[5]);
	strc->start_time = get_time();
	strc->phils = make_philos(strc->nmb_of_philos);
	if (!strc->phils)
		return (free(strc), NULL);
	return (strc);
}

int	check_input(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (philo_error("Incorrect nmb of arguments.\n\
Please input either four or five arguments.\n"), 1);
	if (arg_to_int(argv[1]) < 1 || arg_to_int(argv[2]) < 1 || arg_to_int(argv[3]
		) < 1 || arg_to_int(argv[4]) < 1 || arg_to_int(argv[5]) < 0)
		return (philo_error("Invalid argument value.\n\
Please input only posisitve integers.\n"), 1);
	return (0);
}
