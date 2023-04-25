/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 13:22:19 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	*take_forks(t_philos *philos, size_t id)
{
	size_t	id2;

	if (id == 0)
		id2 = philos->number_of_philos - 1;
	else
		id2 = id - 1;
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
		philo_eat(philos, id, id2);
	return (NULL);
}

void	*philo_eat(t_philos *philos, size_t id, size_t id2)
{
	unsigned long long	start_time;
	unsigned long long	curr_time;

	start_time = get_time();
	printf("%llu %lu %s", get_time() - philos->start_time,
		philos->thinker[id]->philo_id, "is eating\n");
	curr_time = get_time();
	while (curr_time - start_time <= philos->time_to_eat)
	{
		if (philos->thinker[id]->life == false)
			return (NULL);
		usleep(1000);
		curr_time = get_time();
	}
	philos->thinker[id]->last_supper = get_time() - philos->start_time;
	philos->thinker[id]->fork = 0;
	philos->thinker[id2]->fork = 0;
	philo_sleep(philos, id);
	return (NULL);
}

void	*philo_sleep(t_philos *philos, size_t id)
{
	unsigned long long	start_time;
	unsigned long long	curr_time;

	start_time = get_time();
	printf("%llu %lu %s", get_time() - philos->start_time,
		philos->thinker[id]->philo_id, "is sleeping\n");
	curr_time = get_time();
	while (curr_time - start_time <= philos->time_to_sleep)
	{
		if (philos->thinker[id]->life == false)
			return (NULL);
		usleep(1000);
		curr_time = get_time();
	}
	printf("%llu %lu %s", get_time() - philos->start_time,
		philos->thinker[id]->philo_id, "is thinking\n");
	return (NULL);
}

void	*philo_thread(void *arg)
{
	t_philos			*philos;
	static size_t		i;
	size_t				id;
	unsigned long long	time;
	unsigned long long	time2;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	time = (get_time() - philos->start_time);
	id = i;
	i++;
	pthread_mutex_unlock(&philos->lock);
	while (true)
	{
		pthread_mutex_lock(&philos->lock);
		time2 = (get_time() - philos->start_time);
		if (philos->thinker[id]->life == false)
			return (pthread_mutex_unlock(&philos->lock), NULL);
		if (time2 >= (time + 10))
		{
			printf("runtime: %llums by philosopher %lu\n", time2,
				philos->thinker[id]->philo_id);
			time = time2;
		}
		pthread_mutex_unlock(&philos->lock);
	}
	return (NULL);
}
