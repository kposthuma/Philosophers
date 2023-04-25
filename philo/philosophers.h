/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: kposthum <kposthum@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/19 12:55:25 by kposthum      #+#    #+#                 */
/*   Updated: 2023/04/25 13:22:00 by kposthum      ########   odam.nl         */
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

typedef struct s_thinker
{
	size_t				philo_id;
	int					fork;
	unsigned long long	last_supper;
	bool				life;
}	t_thinker;

typedef struct s_philos
{
	size_t				number_of_philos;
	size_t				time_to_die;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				number_of_meals;
	unsigned long long	start_time;
	pthread_mutex_t		lock;
	t_thinker			**thinker;
}	t_philos;

// main.c
t_thinker			**make_philos(size_t num);
t_philos			*init_philos(char **argv);
int					check_input(int argc, char **argv);

// philo_utils.c
void				philo_error(char *message);
int					arg_to_int(char *arg);
unsigned long long	get_time(void);

// philosophers.c
void				*philo_thread(void *arg);
void				*take_forks(t_philos *philos, size_t id);
void				*philo_eat(t_philos *philos, size_t id, size_t id2);
void				*philo_sleep(t_philos *philos, size_t id);

// philo_death.c
void				death(t_philos *philos);
void				*is_dead(void *arg);
#endif