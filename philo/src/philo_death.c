/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_death.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:32:58 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/02 15:14:26 by kposthum      ########   odam.nl         */
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

	philos = (t_philos *)arg;
	while (true)
	{
		i = 0;
		pthread_mutex_lock(&philos->lock);
		now = get_time();
		while (i < philos->number_of_philos)
		{
			if (now - philos->thinker[i]->last_supper >= philos->time_to_die)
			{
				printf("%llu %lu %s", get_time() - philos->start_time,
					philos->thinker[i]->philo_id, "died\n");
				death(philos);
			}
			if (philos->thinker[i]->life == false)
			{
				pthread_mutex_unlock(&philos->lock);
				return (NULL);
			}
			i++;
		}
		pthread_mutex_unlock(&philos->lock);
		usleep(100000);
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
		i = 0;
		pthread_mutex_lock(&philos->lock);
		while (i < philos->number_of_philos)
		{
			if (philos->thinker[i]->meals_eaten == philos->number_of_meals && philos->thinker[i]->finished != true)
			{
				philos->thinker[i]->finished = true;
				end++;
				if (philos->thinker[i]->life == false)
				{
					pthread_mutex_unlock(&philos->lock);
					return (NULL);
				}
			}
			i++;
		}
		if (end == philos->number_of_philos)
		{
			death(philos);
			pthread_mutex_unlock(&philos->lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philos->lock);
		usleep(100);
	}
	return (NULL);
}
