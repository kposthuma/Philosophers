/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 12:55:25 by kposthum      #+#    #+#                 */
/*   Updated: 2023/05/10 16:52:56 by kposthum      ########   odam.nl         */
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
	size_t			nmb_of_philos;
	t_time			time_to_die;
	t_time			time_to_eat;
	t_time			time_to_sleep;
	size_t			nmb_of_meals;
	t_time			start_time;
	pthread_mutex_t	lock;
	t_thinker		**phils;
}	t_philos;

// main.c
t_thinker	**make_philos(size_t num);
t_philos	*init_philos(char **argv);
int			check_input(int argc, char **argv);

// philo_utils.c
void		philo_liberation(t_philos *strc);
void		free_thinkers(t_thinker **phils, size_t count);
void		philo_error(char *message);
t_time		get_time(void);
int			arg_to_int(char *arg);

// philosophers.c
void		fork_message(t_philos *strc, size_t f_id, size_t p_id, int value);
bool		take_forks(t_philos *strc, size_t id, size_t id2);
void		philo_loop(t_philos *strc, size_t id, size_t id2);
void		*philo_thread(void *arg);
void		even_wait(t_philos *strc);

// philo_eat_sleep.c
bool		finished_action(t_time start_time, t_time duration);
bool		action_loop(t_philos *strc, size_t id, t_time duration);
void		*philo_eat(t_philos *pstrc, size_t id, size_t id2);
void		*philo_sleep(t_philos *strc, size_t id);

// philo_death.c
void		death(t_philos *strc);
bool		death_loop(t_philos *strc, size_t count);
void		*is_dead(void *arg);
bool		done_eating(t_philos *strc, size_t count);
void		*has_eaten(void *arg);
#endif