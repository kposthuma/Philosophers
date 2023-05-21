/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_forks.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 11:39:29 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/21 13:28:03 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	fork_message(t_philos *strc, size_t f_id, size_t p_id, int value)
{
	pthread_mutex_lock(&strc->lock);
	if (strc->phils[f_id]->fork == 0 && strc->phils[p_id]->life == true)
	{
		printf("%llu\t%lu\thas taken a fork\n", get_time() - strc
			->start_time, strc->phils[p_id]->philo_id);
		strc->phils[f_id]->fork = value;
	}
	pthread_mutex_unlock(&strc->lock);
}

void	take_forks(t_philos *strc, size_t id, size_t id2)
{	
	int	fork_one;
	int	fork_two;

	pthread_mutex_lock(&strc->phils[id]->fork_lock);
	if (id != id2)
		pthread_mutex_lock(&strc->phils[id2]->fork_lock);
	if (strc->phils[id2]->fork != 1 && strc->phils[id]->fork == 0)
		fork_message(strc, id, id, 1);
	if (strc->phils[id]->fork != 2 && strc->phils[id2]->fork == 0)
		fork_message(strc, id2, id, 2);
	fork_two = strc->phils[id2]->fork;
	fork_one = strc->phils[id]->fork;
	if (fork_one == 1 && fork_two == 2)
	{
		philo_eat(strc, id, id2);
		pthread_mutex_unlock(&strc->phils[id]->fork_lock);
		pthread_mutex_unlock(&strc->phils[id2]->fork_lock);
		philo_sleep(strc, id);
	}
	else
	{
		pthread_mutex_unlock(&strc->phils[id]->fork_lock);
		if (id != id2)
			pthread_mutex_unlock(&strc->phils[id2]->fork_lock);
	}
}
