/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_eat_sleep.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 16:15:56 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/16 15:29:45 by kposthum      ########   odam.nl         */
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

bool	action_loop(t_philos *strc, size_t id, t_time duration)
{
	bool	finished;
	bool	living;
	t_time	start_time;

	start_time = get_time();
	finished = finished_action(start_time, duration);
	while (finished != true)
	{
		pthread_mutex_lock(&strc->lock);
		living = strc->phils[id]->life;
		pthread_mutex_unlock(&strc->lock);
		if (living == false)
			return (false);
		finished = finished_action(start_time, duration);
		usleep(200);
	}
	return (true);
}

void	*philo_eat(t_philos *strc, size_t id, size_t id2)
{
	bool	life;

	pthread_mutex_lock(&strc->lock);
	life = strc->phils[id]->life;
	strc->phils[id]->last_supper = get_time();
	pthread_mutex_unlock(&strc->lock);
	if (life == true)
		printf("%llu\t%lu\tis eating\n", get_time() - strc->start_time,
			strc->phils[id]->philo_id);
	life = action_loop(strc, id, strc->time_to_eat);
	if (life == false)
		return (NULL);
	pthread_mutex_lock(&strc->phils[id]->fork_lock);
	strc->phils[id]->fork = 0;
	pthread_mutex_unlock(&strc->phils[id]->fork_lock);
	pthread_mutex_lock(&strc->phils[id2]->fork_lock);
	strc->phils[id2]->fork = 0;
	pthread_mutex_unlock(&strc->phils[id2]->fork_lock);
	pthread_mutex_lock(&strc->lock);
	strc->phils[id]->meals_eaten++;
	pthread_mutex_unlock(&strc->lock);
	return (philo_sleep(strc, id), NULL);
}

void	*philo_sleep(t_philos *strc, size_t id)
{
	bool	life;

	pthread_mutex_lock(&strc->lock);
	life = strc->phils[id]->life;
	pthread_mutex_unlock(&strc->lock);
	if (life == true)
		printf("%llu\t%lu\tis sleeping\n", get_time() - strc->start_time,
			strc->phils[id]->philo_id);
	life = action_loop(strc, id, strc->time_to_sleep);
	if (life == true)
		printf("%llu\t%lu\tis thinking\n", get_time() - strc->start_time,
			strc->phils[id]->philo_id);
	return (NULL);
}
