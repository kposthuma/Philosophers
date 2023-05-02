/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_eat_sleep.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 16:15:56 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/02 17:21:03 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	*philo_eat(t_philos *philos, size_t id, size_t id2)
{
	t_time	now;

	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	now = get_time() - philos->start_time;
	printf("%llu %lu %s", now, philos->thinker[id]->philo_id, "is eating\n");
	pthread_mutex_unlock(&philos->lock);
	if (action_loop(philos, id, philos->time_to_eat) == false)
		return (NULL);
	pthread_mutex_lock(&philos->lock);
	philos->thinker[id]->last_supper = get_time();
	philos->thinker[id]->fork = 0;
	philos->thinker[id2]->fork = 0;
	philos->thinker[id]->meals_eaten++;
	pthread_mutex_unlock(&philos->lock);
	return (philo_sleep(philos, id), NULL);
}

void	*philo_sleep(t_philos *philos, size_t id)
{
	t_time	now;

	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	now = get_time() - philos->start_time;
	printf("%llu %lu %s", now, philos->thinker[id]->philo_id, "is sleeping\n");
	pthread_mutex_unlock(&philos->lock);
	if (action_loop(philos, id, philos->time_to_sleep) == false)
		return (NULL);
	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	now = get_time() - philos->start_time;
	printf("%llu %lu %s", now, philos->thinker[id]->philo_id, "is thinking\n");
	pthread_mutex_unlock(&philos->lock);
	return (NULL);
}
