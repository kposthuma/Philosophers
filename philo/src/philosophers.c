/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 12:33:40 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

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
	printf("runtime: %llums by philosopher %lu\n",
		time, philos->thinker[id]->philo_id);
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
		usleep(4000);
	}
	return (NULL);
}
