/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_death.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:32:58 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/07 14:20:14 by kposthum      ########   odam.nl         */
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

size_t	death_loop(t_philos *philos, size_t count)
{
	size_t	i;
	t_time	now;

	i = 0;
	while (i < count)
	{
		now = get_time();
		if (now - philos->thinker[i]->last_supper
			>= philos->thinker[i]->time_to_die)
		{
			pthread_mutex_lock(&philos->lock);
			death(philos);
			return (pthread_mutex_unlock(&philos->lock), i + 1);
		}
		if (philos->thinker[i]->life == false)
			return (i + 1);
		i++;
	}
	return (0);
}

void	*is_dead(void *arg)
{
	t_philos	*philos;
	size_t		count;
	size_t		id;
	t_time		tod;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	count = philos->number_of_philos;
	pthread_mutex_unlock(&philos->lock);
	usleep(100);
	id = 0;
	while (id == 0)
	{
		id = death_loop(philos, count);
		usleep(100);
	}
	usleep(100);
	tod = get_time() - philos->thinker[id]->start_time;
	if (done_eating(philos, count) == false)
		printf("%llu %lu died\n", tod, philos->thinker[id]->philo_id);
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
		usleep(10);
	}
	return (NULL);
}
