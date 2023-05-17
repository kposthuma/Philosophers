/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_death.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:32:58 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/17 11:31:28 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	death(t_philos *strc)
{
	size_t	i;

	i = 0;
	while (i < strc->nmb_of_philos)
	{
		strc->phils[i]->life = false;
		i++;
	}
}

bool	death_loop(t_philos *strc, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_lock(&strc->lock);
		if (get_time() - strc->phils[i]->last_supper >= strc->time_to_die)
		{
			death(strc);
			printf("%llu\t%lu\tdied\n", get_time() - strc->start_time,
				strc->phils[i]->philo_id);
			return (pthread_mutex_unlock(&strc->lock), false);
		}
		pthread_mutex_unlock(&strc->lock);
		pthread_mutex_lock(&strc->lock);
		if (strc->phils[i]->life == false)
			return (pthread_mutex_unlock(&strc->lock), false);
		pthread_mutex_unlock(&strc->lock);
		i++;
	}
	return (true);
}

void	*is_dead(void *arg)
{
	t_philos	*strc;
	size_t		count;

	strc = (t_philos *)arg;
	count = strc->nmb_of_philos;
	while (death_loop(strc, count) == true)
		usleep(200);
	return (NULL);
}

bool	done_eating(t_philos *strc, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (strc->phils[i]->finished == false)
			return (false);
		i++;
	}
	pthread_mutex_lock(&strc->lock);
	death(strc);
	pthread_mutex_unlock(&strc->lock);
	return (true);
}

void	*has_eaten(void *arg)
{
	t_philos	*strc;
	size_t		i;
	bool		done;

	strc = (t_philos *)arg;
	while (true)
	{
		i = 0;
		while (i < strc->nmb_of_philos)
		{
			pthread_mutex_lock(&strc->lock);
			if (strc->phils[i]->meals_eaten >= strc->nmb_of_meals
				&& strc->phils[i]->finished != true)
				strc->phils[i]->finished = true;
			pthread_mutex_unlock(&strc->lock);
			i++;
		}
		done = done_eating(strc, strc->nmb_of_philos);
		pthread_mutex_lock(&strc->lock);
		if (done == true || strc->phils[0]->life == false)
			return (pthread_mutex_unlock(&strc->lock), NULL);
		pthread_mutex_unlock(&strc->lock);
		usleep(200);
	}
	return (NULL);
}
