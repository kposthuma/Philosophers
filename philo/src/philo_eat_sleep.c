/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_eat_sleep.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 16:15:56 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/07 13:00:02 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

bool	finished_action(t_time start_time, t_time duration)
{	
	t_time	curr_time;

	curr_time = get_time();
	if (curr_time - start_time < duration)
		return (false);
	return (true);
}

bool	action_loop(t_philos *philos, size_t id, t_time duration)
{
	bool	finished;
	t_time	start_time;

	start_time = get_time();
	finished = finished_action(start_time, duration);
	while (finished != true)
	{
		if (philos->thinker[id]->life == false)
			return (false);
		finished = finished_action(start_time, duration);
		usleep(10);
	}
	return (true);
}

void	*philo_eat(t_philos *philos, size_t id, size_t id2)
{
	t_time	start_time;
	t_time	now;
	bool	life;

	start_time = get_time();
	if (philos->thinker[id]->life == false)
		return (NULL);
	now = get_time() - philos->thinker[id]->start_time;
	printf("%llu %lu is eating\n", now, philos->thinker[id]->philo_id);
	philos->thinker[id]->last_supper = get_time();
	life = action_loop(philos, id, philos->thinker[id]->time_to_eat);
	if (life == false)
		return (NULL);
	pthread_mutex_lock(&philos->lock);
	philos->thinker[id]->fork = 0;
	philos->thinker[id2]->fork = 0;
	pthread_mutex_unlock(&philos->lock);
	philos->thinker[id]->meals_eaten++;
	return (philo_sleep(philos, id), NULL);
}

void	*philo_sleep(t_philos *philos, size_t id)
{
	t_time	start_time;
	t_time	now;
	bool	life;

	start_time = get_time();
	if (philos->thinker[id]->life == false)
		return (NULL);
	now = get_time() - philos->thinker[id]->start_time;
	printf("%llu %lu is sleeping\n", now, philos->thinker[id]->philo_id);
	life = action_loop(philos, id, philos->thinker[id]->time_to_sleep);
	if (life == false)
		return (NULL);
	if (philos->thinker[id]->life == false)
		return (NULL);
	now = get_time() - philos->thinker[id]->start_time;
	printf("%llu %lu is thinking\n", now, philos->thinker[id]->philo_id);
	return (NULL);
}
