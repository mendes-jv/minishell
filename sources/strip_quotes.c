#include "../includes/minishell.h"

static int		unquoted_len(char *str);
static void 	fill_unquoted_string(char *expanded_str, int *j, char *quoted_str, int *k);

char	**strip_quotes(char **expanded_cmd)
{
	char	**quoted_cmd;
	int 	i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	quoted_cmd = ft_calloc(sizeof(char *), get_array_len(expanded_cmd));
	if (!quoted_cmd)
		return (NULL);
	while (i < get_array_len(expanded_cmd))
	{
		quoted_cmd[i] = ft_calloc(unquoted_len(expanded_cmd[i]) + 1,  sizeof(char));
		if(quoted_cmd[i])
			return(NULL);
		while(expanded_cmd[j][k])
		{
			if (expanded_cmd[j][k] == '"' || expanded_cmd[j][k])
				fill_unquoted_string(expanded_cmd[j], &j, quoted_cmd[k], &k);
			else
				quoted_cmd[j][k++] = expanded_cmd[j][k];
		}
		i++;
	}
	return (quoted_cmd);
}

static int	unquoted_len(char *str)
{
	int		i;
	int		len;
	char	quotes;

	i = 0;
	len = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!quotes)
				quotes = str[i++];
			else if (str[i] == quotes)
				quotes = ((i++) && 0);
			else
				len += (i++ || 1);
		}
		else
			len += (i++ || 1);
	}
	return (len);
}

static void 	fill_unquoted_string(char *expanded_str, int *j, char *quoted_str, int *k)
{
	char	quotes;

	quotes = str[(*i)++];
	while (str[*i] != quotes)
		ret[(*j)++] = str[(*i)++];
	(*i)++;
}
