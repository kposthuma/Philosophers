/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 12:55:25 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/02 17:04:26 by kposthum      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include<unistd.h>
# include<stdlib.h>
# include<stdio.h>
# include<string.h>
# include<sys/time.h>
# include<pthread.h>
# include<stdbool.h>

# ifndef _T_TIME
#  define _T_TIME

typedef unsigned long long	t_time;
# endif

typedef struct s_thinker
{
	size_t	philo_id;
	int		fork;
	t_time	last_supper;
	bool	life;
	size_t	meals_eaten;
	bool	finished;
}	t_thinker;

typedef struct s_philos
{
	size_t			number_of_philos;
	t_time			time_to_die;
	t_time			time_to_eat;
	t_time			time_to_sleep;
	size_t			number_of_meals;
	t_time			start_time;
	pthread_mutex_t	lock;
	t_thinker		**thinker;
}	t_philos;

// main.c
t_thinker	**make_philos(size_t num);
t_philos	*init_philos(char **argv);
int			check_input(int argc, char **argv);

// philo_utils.c
bool		finished_action(t_time start_time, t_time duration);
bool		action_loop(t_philos *philos, size_t id, t_time duration);
void		philo_error(char *message);
int			arg_to_int(char *arg);
t_time		get_time(void);

// philosophers.c
void		*philo_thread(void *arg);
bool		take_forks(t_philos *philos, size_t id, size_t id2);

void		*philo_eat(t_philos *philos, size_t id, size_t id2);

void		*philo_sleep(t_philos *philos, size_t id);

// philo_death.c
void		death(t_philos *philos);
void		*is_dead(void *arg);
void		*has_eaten(void *arg);
#endif