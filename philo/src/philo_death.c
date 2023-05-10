/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_death.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:32:58 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/10 13:32:03 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	death(t_philos *philos)
{
	size_t	i;

	i = 0;
	pthread_mutex_lock(&philos->lock);
	while (i < philos->number_of_philos)
	{
		philos->thinker[i]->life = false;
		i++;
	}
	pthread_mutex_unlock(&philos->lock);
}

bool	death_loop(t_philos *philos, size_t count)
{
	size_t	i;
	t_time	now;
	t_time	last_supper;

	i = 0;
	while (i < count)
	{
		now = get_time();
		pthread_mutex_lock(&philos->lock);
		last_supper = philos->thinker[i]->last_supper;
		pthread_mutex_unlock(&philos->lock);
		if (now - last_supper >= philos->thinker[i]->time_to_die)
		{
			death(philos);
			printf("%llu\t%lu\tdied\n", get_time() - philos->thinker[i]
				->start_time, philos->thinker[i]->philo_id);
			return (false);
		}
		if (philos->thinker[i]->life == false)
			return (false);
		i++;
	}
	return (true);
}

void	*is_dead(void *arg)
{
	t_philos	*philos;
	size_t		count;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	count = philos->number_of_philos;
	pthread_mutex_unlock(&philos->lock);
	while (death_loop(philos, count) == true)
		usleep(200);
	return (NULL);
}

bool	done_eating(t_philos *philos, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (philos->thinker[i]->finished == false)
			return (false);
		i++;
	}
	death(philos);
	return (true);
}

void	*has_eaten(void *arg)
{
	t_philos	*philos;
	size_t		i;
	size_t		count;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	count = philos->number_of_philos;
	pthread_mutex_unlock(&philos->lock);
	while (true)
	{
		i = 0;
		while (i < count)
		{
			if (philos->thinker[i]->meals_eaten >= philos->number_of_meals
				&& philos->thinker[i]->finished != true)
				philos->thinker[i]->finished = true;
			i++;
		}
		if (done_eating(philos, count) == true
			|| philos->thinker[0]->life == false)
			return (NULL);
		usleep(200);
	}
	return (NULL);
}
