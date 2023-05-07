/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/07 16:02:44 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	fork_message(t_philos *philos, size_t id)
{
	t_time	now;

	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id]->fork == 0)
	{
		philos->thinker[id]->fork = 1;
		pthread_mutex_unlock(&philos->lock);
		now = get_time() - philos->start_time;
		printf("%llu %lu has taken a fork\n", now,
			philos->thinker[id]->philo_id);
	}
	else
		pthread_mutex_unlock(&philos->lock);

}

bool	take_forks(t_philos *philos, size_t id, size_t id2)
{	
	t_time	now;

	if (philos->thinker[id]->life == false)
		return (NULL);
	fork_message(philos, id);
	if (philos->thinker[id]->life == false)
		return (NULL);
	pthread_mutex_lock(&philos->lock);
	if (philos->thinker[id2]->fork == 0)
	{
		philos->thinker[id2]->fork = 2;
		pthread_mutex_unlock(&philos->lock);
		now = get_time() - philos->start_time;
		printf("%llu %lu has taken a fork\n", now,
			philos->thinker[id]->philo_id);
	}
	else
		pthread_mutex_unlock(&philos->lock);
	if (philos->thinker[id]->fork == 1 && philos->thinker[id2]->fork == 2)
		return (true);
	return (false);
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

void	even_wait(t_philos *philos, size_t id)
{
	t_time	wait;

	wait = get_time() - philos->thinker[id]->start_time;
	while (wait < philos->thinker[id]->time_to_eat)
	{
		wait = get_time() - philos->thinker[id]->start_time;
		usleep(10);
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
	pthread_mutex_unlock(&philos->lock);
	printf("%llu %lu %s", get_time() - philos->thinker[id]->start_time,
		philos->thinker[id]->philo_id, "is thinking\n");
	if (id % 2 == 1)
		even_wait(philos, id);
	philo_loop(philos, id, id2);
	return (NULL);
}
