/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 00:41:58 by aycami            #+#    #+#             */
/*   Updated: 2025/08/19 14:54:25 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int print_action(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->die_mutex);
	if (philo->die_flag)
	{
		pthread_mutex_unlock(&philo->die_mutex);
		return 0;
	}
	pthread_mutex_unlock(&philo->die_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_timestamp(philo->data), philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
	return 1;
}

int think(t_philo *philo)
{
	if(!print_action(philo, "is thinking"))
		return 0;
	return 1;
}

int take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if(!print_action(philo, "has taken a fork"))
	{
	    pthread_mutex_unlock(philo->left_fork);
	    return 0;
	}
	pthread_mutex_lock(philo->right_fork);
	if(!print_action(philo, "has taken a fork"))
	{
	    pthread_mutex_unlock(philo->left_fork);
	    pthread_mutex_unlock(philo->right_fork);
	    return 0;
	}
	return 1;
}

int eat(t_philo *philo)
{
    if(!print_action(philo, "is eating"))
        return 0;
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal = get_timestamp(philo->data);
    philo->meal_sum++;
    pthread_mutex_unlock(&philo->meal_mutex);
    usleep(philo->data->eat_time * 1000);
	return 1;
}

void release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

int go_sleep(t_philo *philo)
{
	if(!print_action(philo, "is sleeping"))
		return 0;
	usleep(philo->data->sleep_time * 1000);
	return 1;
}

void *routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	while (1)
	{
		if(!think(philo))
			break;
		if(!take_forks(philo))
			break;
		if(!eat(philo))
			break;
		release_forks(philo);
		if(!go_sleep(philo))
			break;
	}
	return NULL;
} 