/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_init.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/16 14:21:21 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/17 11:16:05 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

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
		pthread_mutex_init(&phils[i]->fork_lock, NULL);
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