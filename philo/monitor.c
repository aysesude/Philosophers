/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:11:25 by aycami            #+#    #+#             */
/*   Updated: 2025/08/19 14:54:29 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int control(int i, int *finished, t_data *data)
{
	pthread_mutex_lock(&data->philo[i]->meal_mutex);
	long since_last_meal = get_timestamp(data) - data->philo[i]->last_meal;

	if (since_last_meal > data->die_time)
	{
		print_action(data->philo[i], "died");
		pthread_mutex_lock(&data->philo[i]->die_mutex);
		data->philo[i]->die_flag = 1;
		pthread_mutex_unlock(&data->philo[i]->die_mutex);
		for (int j = 0; j < data->count; j++)
		{
			pthread_mutex_lock(&data->philo[j]->die_mutex);
			data->philo[j]->die_flag = 1;
			pthread_mutex_unlock(&data->philo[j]->die_mutex);
		}
		pthread_mutex_unlock(&data->philo[i]->meal_mutex);
		return (1);
	}
	if (data->meals == -1 || data->philo[i]->meal_sum < data->meals)
		*finished = 0;
	pthread_mutex_unlock(&data->philo[i]->meal_mutex);
	return (0);
}

void *monitor(void *arg)
{
	t_data *data = (t_data *)arg;
	int i;
	int finished;

	while (1)
	{
		finished = 1;
		i = 0;
		while (i < data->count)
		{
			if (control(i, &finished, data))
				return NULL;
			i++;
		}
		if (data->meals != -1 && finished == 1)
		{
			for (int j = 0; j < data->count; j++)
			{
				pthread_mutex_lock(&data->philo[j]->die_mutex);
				data->philo[j]->die_flag = 1;
				pthread_mutex_unlock(&data->philo[j]->die_mutex);
			}
			return NULL;
		}  
		usleep(1000);
	}
}
