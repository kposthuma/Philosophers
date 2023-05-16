/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 12:30:35 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/16 14:30:16 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	philo_liberation(t_philos *strc)
{
	if (strc->phils)
		free_thinkers(strc->phils, strc->nmb_of_philos);
	free(strc);
}

void	free_thinkers(t_thinker **phils, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&phils[i]->fork_lock);
		free(phils[i]);
		i++;
	}
	free(phils);
}

void	philo_error(char *message)
{
	if (!message)
		printf("Memory allocation error\n");
	else
		printf("%s", message);
}

t_time	get_time(void)
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	return ((curr.tv_sec * 1000 + curr.tv_usec / 1000));
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
