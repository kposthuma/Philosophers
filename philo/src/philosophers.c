/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/23 11:57:15 by kposthum      ########   odam.nl         */
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
	t_philos		*philos;
	static size_t	i;
	int				x;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	x = i;
	printf("runtime: %llums in thread %lu\n",
		(get_time() - philos->start_time), philos->thinker[x]->philo_id);
	i++;
	pthread_mutex_unlock(&philos->lock);
	return (NULL);
}
