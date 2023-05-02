/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_eat_sleep.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 16:15:56 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/02 18:35:08 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	*philo_eat(t_philos *philos, size_t id, size_t id2)
{
	t_time	start_time;
	t_time	now;
	bool	life;

	start_time = get_time();
	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	now = get_time() - philos->start_time;
	printf("%llu %lu is eating\n", now, philos->thinker[id]->philo_id);
	pthread_mutex_unlock(&philos->lock);
	life = action_loop(philos, id, philos->time_to_eat);
	if (life == false)
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
	t_time	start_time;
	t_time	now;
	bool	life;

	start_time = get_time();
	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	now = get_time() - philos->start_time;
	printf("%llu %lu is sleeping\n", now, philos->thinker[id]->philo_id);
	pthread_mutex_unlock(&philos->lock);
	life = action_loop(philos, id, philos->time_to_sleep);
	if (life == false)
		return (NULL);
	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->life == false)
		return (pthread_mutex_unlock(&philos->lock), NULL);
	now = get_time() - philos->start_time;
	printf("%llu %lu is thinking\n", now, philos->thinker[id]->philo_id);
	pthread_mutex_unlock(&philos->lock);
	return (NULL);
}

