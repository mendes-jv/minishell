#include "../../includes/minishell.h"

static int	check_option_n(char *command);
static void print_args(int i, int arr_len, char **command);

void	exec_echo(char **command)
{
	int arr_len;
	int i;

	arr_len = get_array_len(command);
	if (arr_len == 1)
		ft_printf("\n");
	else
	{
		i = check_option_n(command[1]);
		print_args(i, arr_len, command);
	}
}

static void print_args(int i, int arr_len, char **command) {
	int x;

	if (i == 0)
		x = 1;
	else
		x = 2;
	while (arr_len > x)
	{
		ft_printf("%s", command[x]);
		if (x + 1 != arr_len)
			ft_printf(" ");
		x++;
	}
	if (x == 1)
		ft_printf("\n");
}

static int	check_option_n(char *command)
{
	int i;
	int y;

	i = 0;
	if (command[0] == '-' && command[1] == 'n') {
		i = 1;
		y = 2;
		while (command[y])
		{
			if (command[y] != 'n')
			{
				i = 0;
				break;
			}
			y++;
		}
	}
	return (i);
}
