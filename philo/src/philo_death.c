/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_death.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:32:58 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 18:14:55 by kposthum      ########   odam.nl         */
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

void	*is_dead(void *arg)
{
	t_philos	*philos;
	size_t		i;
	t_time		now;

	i = 0;
	philos = (t_philos *)arg;
	while (true)
	{
		pthread_mutex_lock(&philos->lock);
		now = get_time();
		while (i < philos->number_of_philos)
		{
			if (now - philos->thinker[i]->last_supper >= philos->time_to_die)
			{
				printf("%llu %lu %s", get_time() - philos->start_time,
					philos->thinker[i]->philo_id, "died\n");
				death(philos);
				return (pthread_mutex_unlock(&philos->lock), NULL);
			}
			i++;
		}
		pthread_mutex_unlock(&philos->lock);
		usleep(1000);
	}
	return (NULL);
}

void	*has_eaten(void *arg)
{
	t_philos	*philos;
	size_t		i;
	size_t		end;

	i = 0;
	end = 0;
	philos = (t_philos *)arg;
	while (true)
	{
		pthread_mutex_lock(&philos->lock);
		while (i < philos->number_of_philos)
		{
			if (philos->thinker[i]->meals_eaten == philos->number_of_meals)
				end++;
			i++;
		}
		if (end == philos->number_of_philos)
			return (pthread_mutex_unlock(&philos->lock), death(philos), NULL);
		pthread_mutex_unlock(&philos->lock);
		usleep(100);
	}
	return (NULL);
}
