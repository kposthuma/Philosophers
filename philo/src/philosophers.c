/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/02 18:25:07 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

bool	take_forks(t_philos *philos, size_t id, size_t id2)
{
	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	if (philos->thinker[id]->fork == 0)
	{
		printf("%llu %lu %s", get_time() - philos->start_time,
			philos->thinker[id]->philo_id, "has taken a fork\n");
		philos->thinker[id]->fork = 1;
	}
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	if (philos->thinker[id2]->fork == 0)
	{
		printf("%llu %lu %s", get_time() - philos->start_time,
			philos->thinker[id]->philo_id, "has taken a fork\n");
		philos->thinker[id2]->fork = 2;
	}
	if (philos->thinker[id]->fork == 1 && philos->thinker[id2]->fork == 2)
		return (pthread_mutex_unlock(&philos->lock), true);
	return (pthread_mutex_unlock(&philos->lock), false);
}

void	philo_loop(t_philos *philos, size_t id, size_t id2)
{
	bool	has_forks;

	while (true)
	{
		if (philos->thinker[id]->life == false)
			break ;
		has_forks = take_forks(philos, id, id2);
		if (philos->thinker[id]->life == false)
			break ;
		if (has_forks == true)
			philo_eat(philos, id, id2);
	}
}

void	*philo_thread(void *arg)
{
	t_philos			*philos;
	static size_t		i;
	size_t				id;
	size_t				id2;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	id = i;
	if (id == 0)
		id2 = philos->number_of_philos - 1;
	else
		id2 = id - 1;
	i++;
	printf("%llu %lu %s", get_time() - philos->start_time,
		philos->thinker[id]->philo_id, "is thinking\n");
	pthread_mutex_unlock(&philos->lock);
	if (id % 2 == 1)
		usleep(10000);
	philo_loop(philos, id, id2);
	return (NULL);
}
