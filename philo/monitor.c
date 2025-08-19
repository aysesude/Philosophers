/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:11:25 by aycami            #+#    #+#             */
/*   Updated: 2025/08/19 23:33:37 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	kill_all_philos(t_data *data)
{
	int	j;

	j = 0;
	while (j < data->count)
	{
		pthread_mutex_lock(&data->philo[j]->die_mutex);
		data->philo[j]->die_flag = 1;
		pthread_mutex_unlock(&data->philo[j]->die_mutex);
		j++;
	}
}

int	control(int i, int *finished, t_data *data)
{
	long	since_last_meal;

	pthread_mutex_lock(&data->philo[i]->meal_mutex);
	since_last_meal = get_timestamp(data) - data->philo[i]->last_meal;
	if (since_last_meal >= data->die_time)
	{
		pthread_mutex_unlock(&data->philo[i]->meal_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%ld %d died\n", get_timestamp(data), data->philo[i]->id + 1);
		pthread_mutex_unlock(&data->print_mutex);
		pthread_mutex_lock(&data->philo[i]->die_mutex);
		data->philo[i]->die_flag = 1;
		pthread_mutex_unlock(&data->philo[i]->die_mutex);
		kill_all_philos(data);
		return (1);
	}
	if (data->meals == -1 || data->philo[i]->meal_sum < data->meals)
		*finished = 0;
	pthread_mutex_unlock(&data->philo[i]->meal_mutex);
	return (0);
}

int	check_philosopher_status(t_data *data, int *finished)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		pthread_mutex_lock(&data->philo[i]->die_mutex);
		if (data->philo[i]->die_flag)
		{
			pthread_mutex_unlock(&data->philo[i]->die_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->philo[i]->die_mutex);
		if (control(i, finished, data))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		finished;

	data = (t_data *)arg;
	while (1)
	{
		finished = 1;
		if (check_philosopher_status(data, &finished))
			return (NULL);
		if (data->meals != -1 && finished == 1)
		{
			kill_all_philos(data);
			return (NULL);
		}
		usleep(100);
	}
}

void	free_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		pthread_mutex_destroy(&data->philo[i]->meal_mutex);
		pthread_mutex_destroy(&data->philo[i]->die_mutex);
		free(data->philo[i]);
		i++;
	}
	i = 0;
	while (i < data->count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}
