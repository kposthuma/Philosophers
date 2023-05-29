/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/20 17:40:29 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/29 12:14:59 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

// void	f(void)
// {
// 	system("leaks philo");
// }

int	main(int argc, char **argv)
{
	t_philos	*strc;
	pthread_t	*ph_thread;
	pthread_t	ml_thread;

	if (check_input(argc, argv) != 0)
		return (1);
	strc = init_philos(argv);
	ph_thread = malloc((arg_to_int(argv[1])) * sizeof(pthread_t));
	if (!ph_thread || !strc)
		return (philo_error(NULL), philo_liberation(strc), free(ph_thread), 1);
	ml_thread = NULL;
	if (make_threads(strc, ph_thread, ml_thread) != true)
		return (philo_error(NULL), philo_liberation(strc), free(ph_thread), 1);
	else
	{
		is_dead(strc);
		join_threads(strc, ph_thread, ml_thread);
	}
	pthread_mutex_destroy(&strc->lock);
	return (philo_liberation(strc), free(ph_thread), 0);
}

void	join_threads(t_philos *strc, pthread_t *ph_thread, pthread_t ml_thread)
{
	size_t		i;

	i = 0;
	while (i < strc->nmb_of_philos)
		pthread_join(ph_thread[i++], NULL);
	if (strc->nmb_of_meals != 0)
		pthread_join(ml_thread, NULL);
}

void	end_of_philo(t_philos *strc, size_t i, pthread_t *ph_thread)
{
	size_t	j;

	j = 0;
	pthread_mutex_lock(&strc->lock);
	while (j <= i)
	{
		strc->phils[j]->life = false;
		j++;
	}
	pthread_mutex_unlock(&strc->lock);
	while (j >= 0)
	{
		pthread_join(ph_thread[j], NULL);
		j--;
	}
}

bool	make_threads(t_philos *strc, pthread_t *ph_thread, pthread_t ml_thread)
{
	size_t		i;

	i = 0;
	while (i < strc->nmb_of_philos)
	{
		if (pthread_create(&ph_thread[i], NULL, &phil_thr, (void *)strc) != 0)
			return (end_of_philo(strc, i, ph_thread), false);
		i++;
	}
	if (strc->nmb_of_meals != 0)
	{
		if (pthread_create(&ml_thread, NULL, &has_eaten, (void *)strc) != 0)
			return (end_of_philo(strc, i, ph_thread), false);
	}
	return (true);
}

int	check_input(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (philo_error("Incorrect nmb of arguments.\n\
Please input either four or five arguments.\n"), 1);
	if (arg_to_int(argv[1]) < 1 || arg_to_int(argv[2]) < 1 || arg_to_int(argv[3]
		) < 1 || arg_to_int(argv[4]) < 1 || arg_to_int(argv[5]) < 0)
		return (philo_error("Invalid argument value.\n\
Please input only positive integers.\n"), 1);
	return (0);
}
