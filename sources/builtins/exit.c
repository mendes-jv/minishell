/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:07:22 by pmelo-ca          #+#    #+#             */
/*   Updated: 2024/07/18 14:11:32 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool		isnumber(char *string);
static char		*lltoa(long long c);
static void		putnbr_str(long long n, size_t i, size_t len, char *str);
static size_t	ilen(long long number);

void	exec_exit(char **command)
{
	int	arr_len;
	int	status;

	arr_len = get_array_len(command);
	if (arr_len == 1)
	{
		dprintf(1, "exit\n");
		status = 0;
	}
	else
	{
		if (arr_len == 2 && isvalid_num(command[1]))
		{
			dprintf(1, "exit\n");
			status = atoll(command[1]) % 256;
		}
		else if (arr_len != 2 && isvalid_num(command[1]))
		{
			dprintf(2, ERROR_EXIT_MANY_ARGS);
			return ;
		}
		else
		{
			dprintf(2, ERROR_EXIT_INVALID_ARG, command[1]);
			status = 2;
		}
	}
	//TODO: limpar memoria alocada ate agora e validar o que fazer com status quando nao dar exit.
	exit(status);
}

bool	isvalid_num(char *command)
{
	long long	c;
	char		*string;

	if (command[0] == '+' || command[0] == '-')
		command++;
	if (isnumber(command))
	{
		c = atoll(command);
		string = lltoa(c);
		if (ft_strncmp(string, command, ft_strlen(command)) == 0)
		{
			free(string);
			return (true);
		}
		else
		{
			free(string);
			return (false);
		}
	}
	else
		return (false);
}

static bool	isnumber(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (!ft_isdigit(string[i]))
			return (false);
		i++;
	}
	return (true);
}

static char	*lltoa(long long c)
{
	char			*string;
	size_t			length;
	long long int	number;

	number = c;
	length = ilen(number);
	string = ft_calloc(length + 1, sizeof(char));
	if (!string)
		return (NULL);
	if (number < 0)
	{
		number *= -1;
		*string = '-';
		putnbr_str(number, 1, length, string);
	}
	else
		putnbr_str(number, 0, length, string);
	return (string);
}

static void	putnbr_str(long long n, size_t i, size_t len, char *str)
{
	while (i < len)
	{
		str[--len] = (n % 10) + '0';
		n /= 10;
	}
}

static size_t	ilen(long long number)
{
	size_t	counter;

	counter = 0;
	if (number <= 0)
	{
		number *= -1;
		counter++;
	}
	while (number)
	{
		number /= 10;
		counter++;
	}
	return (counter);
}
