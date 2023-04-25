/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:30:35 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 15:47:03 by kposthum      ########   odam.nl         */
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
