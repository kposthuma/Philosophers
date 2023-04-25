/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 15:42:31 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	*philo_sleep(t_philos *philos, size_t id)
{
	t_time	start_time;
	bool	philo_sleep;

	pthread_mutex_lock(&philos->lock);
	start_time = get_time();
	printf("%llu %lu %s", get_time() - philos->start_time,
		philos->thinker[id]->philo_id, "is sleeping\n");
	philo_sleep = finished_action(start_time, philos->time_to_sleep);
	pthread_mutex_unlock(&philos->lock);
	while (philo_sleep != true)
	{
		pthread_mutex_lock(&philos->lock);
		if (philos->thinker[id]->life == false)
			return (pthread_mutex_unlock(&philos->lock), NULL);
		usleep(1000);
		philo_sleep = finished_sleeping(philos, start_time);
		pthread_mutex_unlock(&philos->lock);
	}
	pthread_mutex_lock(&philos->lock);
	printf("%llu %lu %s", get_time() - philos->start_time,
		philos->thinker[id]->philo_id, "is thinking\n");
	pthread_mutex_unlock(&philos->lock);
	return (NULL);
}

void	*philo_eat(t_philos *philos, size_t id, size_t id2)
{
	t_time	start_time;
	bool	philo_meal;

	pthread_mutex_lock(&philos->lock);
	start_time = get_time();
	printf("%llu %lu %s", get_time() - philos->start_time,
		philos->thinker[id]->philo_id, "is eating\n");
	philo_meal = finished_action(start_time, philos->time_to_eat);
	pthread_mutex_unlock(&philos->lock);
	while (philo_meal != true)
	{
		pthread_mutex_lock(&philos->lock);
		if (philos->thinker[id]->life == false)
			return (pthread_mutex_unlock(&philos->lock), NULL);
		usleep(1000);
		philo_meal = finished_action(start_time, philos->time_to_eat);
		pthread_mutex_unlock(&philos->lock);
	}
	pthread_mutex_lock(&philos->lock);
	philos->thinker[id]->last_supper = get_time() - philos->start_time;
	philos->thinker[id]->fork = 0;
	philos->thinker[id2]->fork = 0;
	philos->thinker[id]->meals_eaten++;
	pthread_mutex_unlock(&philos->lock);
	return (philo_sleep(philos, id), NULL);
}

bool	take_forks(t_philos *philos, size_t id, size_t id2)
{
	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->fork == 0)
	{
		printf("%llu %lu %s", get_time() - philos->start_time,
			philos->thinker[id]->philo_id, "has taken a fork\n");
		philos->thinker[id]->fork = 1;
	}
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

void	*philo_thread(void *arg)
{
	t_philos			*philos;
	static size_t		i;
	size_t				id;
	size_t				id2;
	bool				has_forks;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	id = i;
	if (id == 0)
		id2 = philos->number_of_philos - 1;
	else
		id2 = id - 1;
	i++;
	pthread_mutex_unlock(&philos->lock);
	while (true)
	{
		has_forks = take_forks(philos, id, id2);
		if (has_forks == true)
			philo_eat(philos, id, id2);
	}
	return (NULL);
}
