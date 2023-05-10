/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 11:52:07 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/10 13:25:01 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include<philosophers.h>

void	fork_message(t_philos *philos, size_t f_id, size_t p_id, int value)
{
	int		fval;
	bool	life;

	pthread_mutex_lock(&philos->lock);
	fval = philos->thinker[f_id]->fork;
	life = philos->thinker[p_id]->life;
	pthread_mutex_unlock(&philos->lock);
	if (fval == 0 && life == true)
	{
		printf("%llu\t%lu\thas taken a fork\n", get_time() - philos
			->start_time, philos->thinker[p_id]->philo_id);
		pthread_mutex_lock(&philos->lock);
		philos->thinker[f_id]->fork = value;
		pthread_mutex_unlock(&philos->lock);
	}
}

bool	take_forks(t_philos *philos, size_t id, size_t id2)
{	
	int	fork_two;
	int	fork_one;

	pthread_mutex_lock(&philos->lock);
	fork_two = philos->thinker[id2]->fork;
	fork_one = philos->thinker[id]->fork;
	pthread_mutex_unlock(&philos->lock);
	if (fork_two != 1 && fork_one == 0)
		fork_message(philos, id, id, 1);
	if (fork_one != 2 && fork_two == 0)
		fork_message(philos, id2, id, 2);
	if (fork_one == 1 && fork_two == 2)
		return (true);
	return (false);
}

void	philo_loop(t_philos *philos, size_t id, size_t id2)
{
	bool	has_forks;
	bool	living;

	pthread_mutex_lock(&philos->lock);
	living = philos->thinker[id]->life;
	pthread_mutex_unlock(&philos->lock);
	while (living == true)
	{
		pthread_mutex_lock(&philos->lock);
		living = philos->thinker[id]->life;
		pthread_mutex_unlock(&philos->lock);
		if (living == true)
			has_forks = take_forks(philos, id, id2);
		pthread_mutex_lock(&philos->lock);
		living = philos->thinker[id]->life;
		pthread_mutex_unlock(&philos->lock);
		if (living == true && has_forks == true)
			philo_eat(philos, id, id2);
	}
}

void	even_wait(t_philos *philos, size_t id)
{
	t_time	wait;

	wait = get_time() - philos->thinker[id]->start_time;
	while (wait < philos->thinker[id]->time_to_eat - 10)
	{
		wait = get_time() - philos->thinker[id]->start_time;
		usleep(200);
	}
}

void	*philo_thread(void *arg)
{
	t_philos			*philos;
	static size_t		i;
	size_t				id;
	size_t				id2;

	philos = (t_philos *)arg;
	pthread_mutex_lock(&philos->lock);
	id = i;
	if (id == 0)
		id2 = philos->number_of_philos - 1;
	else
		id2 = id - 1;
	i++;
	pthread_mutex_unlock(&philos->lock);
	printf("%llu\t%lu\tis thinking\n", get_time() - philos->thinker[id]
		->start_time, philos->thinker[id]->philo_id);
	if (id % 2 == 1)
		even_wait(philos, id);
	philo_loop(philos, id, id2);
	return (NULL);
}
