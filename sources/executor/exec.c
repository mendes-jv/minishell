#include "../../includes/minishell.h"

int		execute(t_ast *ast, bool piped)
{
	int exit_status;

	if (!ast)
		return (1);
	if (ast->flag == WORD)
		return(exec_simple_command(ast));
	else if (ast->flag == PIPE)
		return(exec_pipeline(ast, true));
	else if (ast->flag == D_AND)
	{
		exit_status = execute(ast->left, false);
		if (!exit_status)
			return(execute(ast->right, false)));
		return(exit_status);
	}
	else if (ast->flag == D_PIPE)
	{
		exit_status = execute(ast->left, false);
		if (exit_status)
			return(exit_status);
		return(execute(ast->right, false)));
	}
	return (exit_status);
}

int	exec_simple_command(t_ast *ast, bool piped)
{
	if (is_builtin(ast->expanded_cmd[0]))
		return (bultin_exec(ast->expanded_cmd, envp)); // validate env
	else
		return (exec_node(ast));
}

