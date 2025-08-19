/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:41:58 by aycami            #+#    #+#             */
/*   Updated: 2025/08/19 23:49:12 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	think(t_philo *philo)
{
	if (!print_action(philo, "is thinking"))
		return (0);
	usleep(1000);
	return (1);
}

int	handle_single_philo(t_philo *philo)
{
	usleep(philo->data->die_time * 1000);
	print_action(philo, "died");
	pthread_mutex_lock(&philo->die_mutex);
	philo->die_flag = 1;
	pthread_mutex_unlock(&philo->die_mutex);
	pthread_mutex_unlock(philo->left_fork);
	philo->has_left = 0;
	return (0);
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	philo->has_left = 1;
	if (!print_action(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(philo->left_fork);
		philo->has_left = 0;
		return (0);
	}
	if (philo->data->count == 1)
		return (handle_single_philo(philo));
	pthread_mutex_lock(philo->right_fork);
	philo->has_right = 1;
	if (!print_action(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(philo->right_fork);
		philo->has_right = 0;
		pthread_mutex_unlock(philo->left_fork);
		philo->has_left = 0;
		return (0);
	}
	return (1);
}

int	eat(t_philo *philo)
{
	if (!print_action(philo, "is eating"))
		return (0);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_timestamp(philo->data);
	philo->meal_sum++;
	pthread_mutex_unlock(&philo->meal_mutex);
	usleep(philo->data->eat_time * 1000);
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->has_left = 0;
	philo->has_right = 0;
	if (philo->data->count > 1 && philo->id % 2 == 0)
		usleep(philo->data->eat_time * 1000 / 2);
	while (1)
	{
		if (!think(philo))
			break ;
		if (!take_forks(philo))
			break ;
		if (!eat(philo))
			break ;
		release_forks(philo);
		if (!go_sleep(philo))
			break ;
	}
	release_forks(philo);
	return (NULL);
}
