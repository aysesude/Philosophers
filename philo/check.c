/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:09:57 by aycami            #+#    #+#             */
/*   Updated: 2025/08/19 11:13:27 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_check(unsigned long long result, const char *str, int sign,
		int i)
{
	if (result > 922337203685477580 || (result == 922337203685477580 && (str[i]
				- '0') > 7))
	{
		if (sign == 1)
			return (-1);
		else
			return (0);
	}
	else
		return (1);
}

static int	ft_white_space(int i, const char *str)
{
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int			i;
	long long	result;
	int			sign;
	int			re;

	i = 0;
	result = 0;
	sign = 1;
	re = 0;
	i += ft_white_space(i, str);
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		re = ft_check(result, str, sign, i);
		if (re == -1 || re == 0)
			return (re);
		result = ((result * 10) + (str[i++] - 48));
	}
	return ((int)(result * sign));
}

void	check_num(char *str)
{
	long long	value;
	int			i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] > 57 || str[i] < 48)
		{
			printf("Error: Not numerical argument\n");
			exit (1);
		}
		i++;
	}
	value = ft_atoi(str);
	if (value < 0 || value > 2147483647 || value < -2147483648)
	{
		printf("Error: Not positive or not integer value\n");
		exit (1);
	}
}

void	check(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		exit (1);
	}
	i = 1;
	while (i < argc)
	{
		check_num(argv[i]);
		i++;
	}
	if (ft_atoi(argv[1]) > 200)
	{
		printf("Error: The number of philosophers cannot exceed 200\n");
		exit (1);
	}
}
