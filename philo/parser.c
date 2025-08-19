/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:09:55 by aycami            #+#    #+#             */
/*   Updated: 2025/08/20 00:29:22 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}

void	assign_forks(t_data *data, int i)
{
	int	left_fork_idx;
	int	right_fork_idx;

	left_fork_idx = i;
	right_fork_idx = (i + 1) % data->count;
	if (left_fork_idx < right_fork_idx)
	{
		data->philo[i]->left_fork = &data->forks[left_fork_idx];
		data->philo[i]->right_fork = &data->forks[right_fork_idx];
	}
	else
	{
		data->philo[i]->left_fork = &data->forks[right_fork_idx];
		data->philo[i]->right_fork = &data->forks[left_fork_idx];
	}
}

void	init_philos(t_data *data)
{
	int	i;

	init_forks(data);
	i = 0;
	while (i < data->count)
	{
		data->philo[i] = malloc(sizeof(t_philo));
		data->philo[i]->id = i;
		data->philo[i]->last_meal = data->start_time;
		data->philo[i]->meal_sum = 0;
		data->philo[i]->data = data;
		assign_forks(data, i);
		data->philo[i]->die_flag = 0;
		pthread_mutex_init(&data->philo[i]->meal_mutex, NULL);
		pthread_mutex_init(&data->philo[i]->die_mutex, NULL);
		i++;
	}
}

void	init_time(t_data *data, char **argv)
{
	data->count = ft_atoi(argv[1]);
	data->die_time = ft_atoi(argv[2]);
	data->eat_time = ft_atoi(argv[3]);
	data->sleep_time = ft_atoi(argv[4]);
	if (argv[5])
		data->meals = ft_atoi(argv[5]);
	else
		data->meals = -1;
	init_philos(data);
}

void	init(t_data *data, char **argv)
{
	int				i;
	struct timeval	time_val;

	gettimeofday(&time_val, NULL);
	data->start_time = (time_val.tv_sec * 1000) + (time_val.tv_usec / 1000);
	pthread_mutex_init(&data->print_mutex, NULL);
	init_time(data, argv);
	i = 0;
	while (i < data->count)
	{
		pthread_create(&data->threads[i], NULL, routine, data->philo[i]);
		i++;
	}
}
