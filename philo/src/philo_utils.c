/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:30:35 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/02 17:06:36 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

bool	finished_action(t_time start_time, t_time duration)
{	
	t_time	curr_time;

	curr_time = get_time();
	if (curr_time - start_time <= duration)
		return (false);
	return (true);
}

bool	action_loop(t_philos *philos, size_t id, t_time duration)
{
	bool	finished;
	t_time	start_time;

	start_time = get_time();
	finished = false;
	while (finished != true)
	{
		pthread_mutex_lock(&philos->lock);
		if (philos->thinker[id]->life == false)
			return (pthread_mutex_unlock(&philos->lock), false);
		finished = finished_action(start_time, duration);
		pthread_mutex_unlock(&philos->lock);
		usleep(1000);
	}
	return (true);
}

void	philo_error(char *message)
{
	printf("%s", message);
}

t_time	get_time(void)
{
	t_time			time;
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return (time = (curr.tv_sec * 1000 + curr.tv_usec / 1000));
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
