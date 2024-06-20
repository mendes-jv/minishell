#include "../../includes/minishell.h"

void	exec_echo(char **command)
{
	int arr_len;
	int i;
	int x;
	int y;

	arr_len = get_array_len(command);
	i = 0;
	if (arr_len == 1)
	{
		printf("\n");
		return;
	}
	if (command[1][0] == '-' && command[1][1] == 'n')
	{
		i = 1;
		y = 2;
		while (command[1][y])
		{
			if (command[1][y] != 'n') {
				i = 0;
				break;
			}
			y++;
		}
	}
	if (arr_len > 1)
	{
		if (i == 1)
		{
			x = 2;
			while(arr_len > x)
			{
				printf("%s", command[x]);
				if (x + 1 != arr_len)
					printf(" ");
				x++;
			}
		}
		else
		{
			x = 1;
			while (arr_len > x)
			{
				printf("%s", command[x]);
				if (x + 1 != arr_len)
					printf(" ");
				x++;
			}
			printf("\n");
		}
	}
}