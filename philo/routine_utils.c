/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:13:21 by aycami            #+#    #+#             */
/*   Updated: 2025/08/19 23:39:25 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_timestamp(t_data *data)
{
	struct timeval	tv;
	long			time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms - data->start_time);
}

int	print_action(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->die_mutex);
	if (philo->die_flag)
	{
		pthread_mutex_unlock(&philo->die_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->die_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_timestamp(philo->data), philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
	return (1);
}

void	release_forks(t_philo *philo)
{
	if (philo->has_left)
	{
		pthread_mutex_unlock(philo->left_fork);
		philo->has_left = 0;
	}
	if (philo->has_right)
	{
		pthread_mutex_unlock(philo->right_fork);
		philo->has_right = 0;
	}
}

int	go_sleep(t_philo *philo)
{
	if (!print_action(philo, "is sleeping"))
		return (0);
	usleep(philo->data->sleep_time * 1000);
	return (1);
}
