#include "../../includes/minishell.h"

int		exec_error_handler(int status, char *message, char *command)
{
	ft_putstr_fd("zapshell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message,2);
	return (status);
}