#include "../../includes/minishell.h"

static char	*validate_access(char *command);
static void	clean_child_data(char **matrix, char *possible_path, char *part_path);
static void	clean_matrix(char **matrix);

char *get_path(char *command, char **env)
{
	char	*part_path;
	char	*possible_path;
	char	**paths;
	int		i;

	i = 0;
	if (ft_strchr(command, '/'))
		return (validate_access(cmd));
	while (ft_strncmp(s_pipex->env[i], "PATH=", 4))
		i++;
	paths = ft_split(s_pipex->env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i++], "/");
		possible_path = ft_strjoin(part_path, cmd);
		if (!(access(possible_path, X_OK)))
		{
			clean_child_data(paths, NULL, part_path);
			return (possible_path);
		}
		clean_child_data(NULL, possible_path, part_path);
	}
	clean_matrix(paths);
	return (NULL);
}

static void	clean_child_data(char **matrix, char *possible_path, char *part_path) // TODO checkifnecessary
{
	if (matrix)
		clean_matrix(matrix);
	if (possible_path)
		free(possible_path);
	if (part_path)
		free(part_path);
}
static void	clean_matrix(char **matrix) //TODO checkifnecessary
{
	int	x;

	x = 0;
	if (!matrix)
		return ;
	while (matrix[x])
		free(matrix[x++]);
	free(matrix);
}

static char	*validate_access(char *command)
{
	if (!(access(command, X_OK)))
		return (command);
	else
		return ("invalid");
}

void reset_redirects(bool piped) //TODO check if is working
{
	if (piped)
		return;
	dup2(STDIN_FILENO, 0);
	dup2(STDOUT_FILENO, 1);
}