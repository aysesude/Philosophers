/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:58:50 by aycami            #+#    #+#             */
/*   Updated: 2025/08/19 15:42:36 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
    int				die_flag;
    int				id;
    int				meal_sum;
    long			last_meal;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t	*forks;
    struct s_data	*data;
    pthread_mutex_t	meal_mutex;
    pthread_mutex_t	die_mutex;
    int				has_left;
    int				has_right;
}                   t_philo;

typedef struct s_data
{
    int				count;
    int				die_time;
    int				eat_time;
    int				sleep_time;
    int				meals;
    long			start_time;
    pthread_t		monitor_thread;
    pthread_t		threads[200];
    pthread_mutex_t	forks[200];
    t_philo			*philo[200];
    pthread_mutex_t	print_mutex;
}                   t_data;

int	ft_atoi(const char *str);
void	check(int argc, char **argv);
long	get_timestamp(t_data *data);
void	init(t_data *data, char **argv);
int		print_action(t_philo *philo, char *msg);
void 	*routine(void *arg);
void 	*monitor(void *arg);
void	free_all(t_data *data);

#endif