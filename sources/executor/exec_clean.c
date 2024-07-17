#include "../../includes/minishell.h"

void clear_ast(t_ast *ast)
{
	if (!ast)
		return;
	clear_ast(ast->left);
	clear_ast(ast->right);
	clear_redirs(ast->redirs); //TODO
	if(ast->cmd)
		free(ast->cmd);
	if(ast->expanded_cmd)
		clear_matrix(ast->expanded_cmd);
	free(ast);
}
