/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/16 15:29:48 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	fork_message(t_philos *strc, size_t f_id, size_t p_id, int value)
{
	int		fval;
	bool	life;

	pthread_mutex_lock(&strc->lock);
	life = strc->phils[p_id]->life;
	pthread_mutex_unlock(&strc->lock);
	pthread_mutex_lock(&strc->phils[f_id]->fork_lock);
	fval = strc->phils[f_id]->fork;
	if (fval == 0 && life == true)
	{
		printf("%llu\t%lu\thas taken a fork\n", get_time() - strc
			->start_time, strc->phils[p_id]->philo_id);
		strc->phils[f_id]->fork = value;
	}
	pthread_mutex_unlock(&strc->phils[f_id]->fork_lock);
}

bool	take_forks(t_philos *strc, size_t id, size_t id2)
{	
	int	fork_one;
	int	fork_two;

	pthread_mutex_lock(&strc->phils[id]->fork_lock);
	fork_one = strc->phils[id]->fork;
	pthread_mutex_unlock(&strc->phils[id]->fork_lock);
	pthread_mutex_lock(&strc->phils[id2]->fork_lock);
	fork_two = strc->phils[id2]->fork;
	pthread_mutex_unlock(&strc->phils[id2]->fork_lock);
	if (fork_two != 1 && fork_one == 0)
		fork_message(strc, id, id, 1);
	if (fork_one != 2 && fork_two == 0)
		fork_message(strc, id2, id, 2);
	if (fork_one == 1 && fork_two == 2)
		return (true);
	return (false);
}

void	philo_loop(t_philos *strc, size_t id, size_t id2)
{
	bool	has_forks;
	bool	living;

	pthread_mutex_lock(&strc->lock);
	living = strc->phils[id]->life;
	pthread_mutex_unlock(&strc->lock);
	while (living == true)
	{
		pthread_mutex_lock(&strc->lock);
		living = strc->phils[id]->life;
		pthread_mutex_unlock(&strc->lock);
		if (living == true)
			has_forks = take_forks(strc, id, id2);
		pthread_mutex_lock(&strc->lock);
		living = strc->phils[id]->life;
		pthread_mutex_unlock(&strc->lock);
		if (living == true && has_forks == true)
			philo_eat(strc, id, id2);
	}
}

void	even_wait(t_philos *strc)
{
	t_time	wait;

	wait = get_time() - strc->start_time;
	while (wait < strc->time_to_eat - 10)
	{
		wait = get_time() - strc->start_time;
		usleep(200);
	}
}

void	*philo_thread(void *arg)
{
	t_philos			*strc;
	static size_t		i;
	size_t				id;
	size_t				id2;

	strc = (t_philos *)arg;
	pthread_mutex_lock(&strc->lock);
	id = i;
	if (id == 0)
		id2 = strc->nmb_of_philos - 1;
	else
		id2 = id - 1;
	i++;
	pthread_mutex_unlock(&strc->lock);
	printf("%llu\t%lu\tis thinking\n", get_time() - strc->start_time,
		strc->phils[id]->philo_id);
	if (id % 2 == 1)
		even_wait(strc);
	philo_loop(strc, id, id2);
	return (NULL);
}
