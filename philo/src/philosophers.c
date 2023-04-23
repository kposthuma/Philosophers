/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/23 17:03:14 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	philo_error(char *message)
{
	printf("%s", message);
}

int	arg_to_int(char *arg)
{
	int	num;
	int	sign;

	if (!arg)
		return (0);
	num = 0;
	sign = 1;
	while (*arg == ' ' || (*arg >= '\t' && *arg <= '\r'))
		arg++;
	if (*arg == '+' || *arg == '-')
	{
		if (*arg == '-')
			sign = sign * -1;
		arg++;
	}
	while (*arg >= '0' && *arg <= '9')
	{
		num = 10 * num + (*arg - '0');
		arg++;
	}
	num = num * sign;
	return (num);
}

unsigned long long	get_time(void)
{
	unsigned long long		time;
	struct timeval			curr;

	gettimeofday(&curr, NULL);
	return (time = (curr.tv_sec * 1000 + curr.tv_usec / 1000));
}

void	*print_time(void *arg)
{
	t_philos			*philos;
	static size_t		i;
	size_t				id;
	unsigned long long	time;
	unsigned long long	time2;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	id = i;
	printf("runtime: %llums in thread %lu\n",
		(get_time() - philos->start_time), philos->thinker[id]->philo_id);
	i++;
	pthread_mutex_unlock(&philos->lock);
	time = 0;
	while (time < philos->time_to_die)
	{
		time2 = (get_time() - philos->start_time);
		if (philos->thinker[id]->life == false)
			return (NULL);
		if (time2 >= (time + 10))
		{
			printf("runtime: %llums in thread %lu\n", time2,
				philos->thinker[id]->philo_id);
			time = time2;
		}
		usleep(4000);
	}
	return (NULL);
}

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

	i = 0;
	philos = (t_philos *)arg;
	while (i < philos->number_of_philos)
	{
		if (get_time() - philos->thinker[i]->last_supper > philos->time_to_die)
		{
			pthread_mutex_lock(&philos->lock);
			printf("%llu %lu %s", get_time() - philos->start_time,
				philos->thinker[i]->philo_id, "died\n");
			death(philos);
			pthread_mutex_unlock(&philos->lock);
			return (NULL);
		}
		i++;
	}
	is_dead((void *)philos);
	return (NULL);
}
