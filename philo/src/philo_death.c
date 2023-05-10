/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_death.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:32:58 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/10 16:48:24 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	death(t_philos *strc)
{
	size_t	i;

	i = 0;
	pthread_mutex_lock(&strc->lock);
	while (i < strc->nmb_of_philos)
	{
		strc->phils[i]->life = false;
		i++;
	}
	pthread_mutex_unlock(&strc->lock);
}

bool	death_loop(t_philos *strc, size_t count)
{
	size_t	i;
	t_time	now;
	t_time	last_supper;

	i = 0;
	while (i < count)
	{
		now = get_time();
		pthread_mutex_lock(&strc->lock);
		last_supper = strc->phils[i]->last_supper;
		pthread_mutex_unlock(&strc->lock);
		if (now - last_supper >= strc->time_to_die)
		{
			death(strc);
			printf("%llu\t%lu\tdied\n", get_time() - strc->start_time,
				strc->phils[i]->philo_id);
			return (false);
		}
		if (strc->phils[i]->life == false)
			return (false);
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
	death(strc);
	return (true);
}

void	*has_eaten(void *arg)
{
	t_philos	*strc;
	size_t		i;
	size_t		count;
	size_t		meals;

	strc = (t_philos *)arg;
	count = strc->nmb_of_philos;
	while (true)
	{
		i = 0;
		while (i < count)
		{
			pthread_mutex_lock(&strc->lock);
			meals = strc->phils[i]->meals_eaten;
			pthread_mutex_unlock(&strc->lock);
			if (meals >= strc->nmb_of_meals && strc->phils[i]->finished != true)
				strc->phils[i]->finished = true;
			i++;
		}
		if (done_eating(strc, count) == true || strc->phils[0]->life == false)
			return (NULL);
		usleep(200);
	}
	return (NULL);
}
