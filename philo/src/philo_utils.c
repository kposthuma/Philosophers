/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:30:35 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 12:31:41 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	philo_error(char *message)
{
	printf("%s", message);
}

unsigned long long	get_time(void)
{
	unsigned long long		time;
	struct timeval			curr;

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
