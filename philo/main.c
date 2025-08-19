/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:10:49 by aycami            #+#    #+#             */
/*   Updated: 2025/08/20 00:24:43 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (check(argc, argv))
		return (1);
	init(&data, argv);
	pthread_create(&data.monitor_thread, NULL, monitor, &data);
	i = 0;
	while (i < data.count)
	{
		pthread_join(data.threads[i], NULL);
		i++;
	}
	pthread_join(data.monitor_thread, NULL);
	free_all(&data);
}
