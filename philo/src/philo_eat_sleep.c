/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_eat_sleep.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 16:15:56 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/10 13:30:21 by kposthum      ########   odam.nl         */
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
	bool	living;
	t_time	start_time;

	start_time = get_time();
	finished = finished_action(start_time, duration);
	while (finished != true)
	{
		pthread_mutex_lock(&philos->lock);
		living = philos->thinker[id]->life;
		pthread_mutex_unlock(&philos->lock);
		if (living == false)
			return (false);
		finished = finished_action(start_time, duration);
		usleep(200);
	}
	return (true);
}

void	*philo_eat(t_philos *philos, size_t id, size_t id2)
{
	bool	life;

	pthread_mutex_lock(&philos->lock);
	life = philos->thinker[id]->life;
	philos->thinker[id]->last_supper = get_time();
	pthread_mutex_unlock(&philos->lock);
	if (life == true)
		printf("%llu\t%lu\tis eating\n", get_time() - philos->thinker[id]
			->start_time, philos->thinker[id]->philo_id);
	life = action_loop(philos, id, philos->thinker[id]->time_to_eat);
	if (life == false)
		return (NULL);
	pthread_mutex_lock(&philos->lock);
	philos->thinker[id]->fork = 0;
	philos->thinker[id2]->fork = 0;
	philos->thinker[id]->meals_eaten++;
	pthread_mutex_unlock(&philos->lock);
	return (philo_sleep(philos, id), NULL);
}

void	*philo_sleep(t_philos *philos, size_t id)
{
	bool	life;

	pthread_mutex_lock(&philos->lock);
	life = philos->thinker[id]->life;
	pthread_mutex_unlock(&philos->lock);
	if (life == true)
		printf("%llu\t%lu\tis sleeping\n", get_time() - philos->thinker[id]
			->start_time, philos->thinker[id]->philo_id);
	life = action_loop(philos, id, philos->thinker[id]->time_to_sleep);
	if (life == true)
		printf("%llu\t%lu\tis thinking\n", get_time() - philos->thinker[id]
			->start_time, philos->thinker[id]->philo_id);
	return (NULL);
}
