#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libraries/libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef ASCII_ART
#  define ASCII_ART "\
                             @@@@@@@@@@@@@@@@@@@@@@@\n\
                        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\
                     @@@@@@@@@@///////////////////@@@@@@@@@@\n\
                  @@@@@@@@/////////////////////////////@@@@@@@@\n\
                @@@@@@@////////////////////////////////////@@@@@@\n\
               @@@@@@///////////////////////////////////////@@@@@@\n\
             @@@@@@/////////@@@@@/////////////////////////////@@@@@\n\
            @@@@@@////////@@@@@@@@/////////////////////////////@@@@@\n\
            @@@@@////////@@@@@@@@@@/////////////////////////////@@@@@\n\
           @@@@@/////////@@@@@@@@@@/////////////////////////////(@@@@@\n\
           @@@@@//////////@@@@@@@////////////////////////////////@@@@@\n\
           @@@@@////////////@@@@@@///////////////////////////////@@@@@\n\
           @@@@@/////////////@@@@@@@/////////////////////////////@@@@@\n\
           @@@@@///////////////@@@@@@@@////////@@@@//////////////@@@@@\n\
            @@@@@/////////////////@@@@@@@@@@/@@@@@@@@@@/////////@@@@@@\n\
             @@@@@///////////////////@@@@@@@@@@@@@@@@@@////////@@@@@@\n\
              @@@@@///////////////////////@@@@@@@@@@@/////////@@@@@@\n\
               @@@@@////////////////////////////////////////@@@@@@\n\
              @@@@@///////////////////////////////////////@@@@@@@\n\
             @@@@@//////////////////////////////////////@@@@@@@@\n\
             @@@@////@@@@@@@//////////////////////@@@@@@@@@\n\
            @@@@@@@@@@@@@@@@@@@@@@@@@@@////@@@@@@@@@@@@@@\n\
           @@@@@@@           @@@@@@@@@@@@@@@@@@@@@@@\n\n\
      __________                   _________.__             .__   .__   \n\
      \\____    /_____   ______    /   _____/|  |__    ____  |  |  |  |  \n\
        /     / \\__  \\  \\____ \\   \\_____  \\ |  |  \\ _/ __ \\ |  |  |  |  \n\
       /     /_  / __ \\_|  |_> >  /        \\|   Y  \\\\  ___/ |  |__|  |__\n\
      /_______ \\(____  /|   __/  /_______  /|___|  / \\___  >|____/|____/\n\
              \\/     \\/ |__|             \\/      \\/      \\/\n\n"
# endif

# ifndef ASCII_GREEN
#  define ASCII_GREEN "\033[0;32m%c"
# endif

# ifndef ASCII_WHITE
#  define ASCII_WHITE "\033[0m%c"
# endif

# ifndef ASCII_RESET
#  define ASCII_RESET "\033[0m%s"
# endif

# ifndef PROMPT
#  define PROMPT "ZapShell -> "
# endif

# ifndef METACHARS
#  define METACHARS "><&|()*"
# endif

# ifndef OPERATORS
#  define OPERATORS "><&|"
# endif

# ifndef QUOTES
#  define QUOTES "'\""
# endif

# ifndef WHITESPACE
#  define WHITESPACE " "
# endif

typedef enum e_flag
{
	WORD,
	PIPE,
	GREATER,
	LESSER,
	D_GREATER,
	D_LESSER,
	D_PIPE,
	D_AND,
	L_PAR,
	R_PAR
}	t_flag;

typedef struct s_token
{
	char	*value;
	t_flag	flag;
}	t_token;

typedef struct s_word_pattern
{
	char	*pattern;
	t_flag	flag;
}	t_word_pattern;

typedef enum e_parse_status
{
	MEMORY_ERROR,
	SYNTAX_ERROR,
	NO_ERROR
}	t_parse_status;

typedef struct s_redir
{
	char	*value;
	char 	**expanded_values;
	int 	heredoc;
	t_flag	flag;
}	t_redir;

typedef struct s_ast
{
	t_flag			flag;
	t_dlist			*redirs;
	char 			*cmd;
	char			**expanded_cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

void 	lexer(char *command_line, t_dlist **words);
void	parser(char *command_line, t_ast **ast);
void	execute(t_ast *ast);
#endif //MINISHELL_H
