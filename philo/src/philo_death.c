/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_death.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:32:58 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/03 11:07:40 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	death(t_philos *philos)
{
	size_t	i;

	i = 0;
	while (i < philos->number_of_philos)
	{
		philos->thinker[i]->life = false;
		i++;
	}
}

bool	death_loop(t_philos *philos)
{
	size_t	i;
	t_time	now;

	i = 0;
	now = get_time();
	pthread_mutex_lock(&philos->lock);
	while (i < philos->number_of_philos)
	{
		if (now - philos->thinker[i]->last_supper >= philos->time_to_die)
		{
			now = get_time() - philos->start_time;
			printf("%llu %lu %s", now, philos->thinker[i]->philo_id,
				"died\n");
			death(philos);
		}
		if (philos->thinker[i]->life == false)
			return (pthread_mutex_unlock(&philos->lock), false);
		i++;
	}
	pthread_mutex_unlock(&philos->lock);
	return (true);
}

void	*is_dead(void *arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	while (death_loop(philos) == true)
		usleep(10000);
	return (NULL);
}

bool	done_eating(t_philos *philos)
{
	size_t	i;

	i = 0;
	while (i < philos->number_of_philos)
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

	philos = (t_philos *)arg;
	while (true)
	{
		i = 0;
		pthread_mutex_lock(&philos->lock);
		while (i < philos->number_of_philos)
		{
			if (philos->thinker[i]->meals_eaten >= philos->number_of_meals
				&& philos->thinker[i]->finished != true)
				philos->thinker[i]->finished = true;
			i++;
		}
		if (done_eating(philos) == true || philos->thinker[1]->life == false)
			return (pthread_mutex_unlock(&philos->lock), NULL);
		pthread_mutex_unlock(&philos->lock);
		usleep(100);
	}
	return (NULL);
}
