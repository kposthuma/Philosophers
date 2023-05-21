/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/21 15:10:58 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	philo_loop(t_philos *strc, size_t id, size_t id2)
{
	bool	living;

	living = true;
	while (living == true)
	{
		pthread_mutex_lock(&strc->lock);
		living = strc->phils[id]->life;
		pthread_mutex_unlock(&strc->lock);
		if (living == true)
			take_forks(strc, id, id2);
	}
}

void	even_wait(t_philos *strc)
{
	t_time	wait;

	wait = get_time() - strc->start_time;
	while (wait < strc->time_to_eat - strc->time_to_eat / 2)
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
	if (i == 0)
		strc->start_time = get_time();
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
