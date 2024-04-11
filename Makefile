# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jovicto2 <jovicto2@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/14 11:17:20 by jovicto2          #+#    #+#              #
#    Updated: 2024/02/14 11:17:23 by jovicto2         ###   ########.org.br    #
#                                                                              #
# **************************************************************************** #

# Variables
NAME					:= minishell

SOURCES_DIR				:= sources/
OBJECTS_DIR				:= objects/
HEADERS_DIR				:= includes/
LIBRARIES_DIR			:= libraries/
LIBFT_DIR				:= $(addprefix $(LIBRARIES_DIR), libft/)
LIBFT_FILE 				:= libft.a

MAKE					:= make
MAKE_LIBS				:= $(MAKE) -sC
CC						:= cc
CFLAGS					:= -Wall -Wextra -Werror -Wunreachable-code -Ofast -g3 -O3
LFLAGS					:= -lreadline -lhistory
MKDIR					:= mkdir -p
RM						:= rm -rf

# Sources
FILES					:= minishell
SOURCES					:= $(addprefix $(SOURCES_DIR), $(addsuffix .c, $(FILES)))
OBJECTS					:= $(addprefix $(OBJECTS_DIR), $(addsuffix .o, $(FILES)))
LIBFT					:= $(addprefix $(LIBFT_DIR), $(LIBFT_FILE))
LIBFT_HEADER			:= $(addprefix $(LIBFT_DIR), includes/libft.h)
HEADERS					:= -I $(HEADERS_DIR)

# Colors
RED   					:= \033[0;31m
GREEN  					:= \033[0;32m
RESET  		 			:= \033[0m

# Rules
.PHONY: all clean fclean re

all:
	@$(MAKE) -s libraries
	@if [ ! -f $(NAME) ]; then \
		$(MAKE) -s $(NAME); \
		if [ -f $(NAME) ]; then \
    		printf "$(GREEN)Compiled $(NAME) successfully!$(RESET)\n"; \
    	else \
    		printf "$(RED)$(NAME) is not compiled yet!$(RESET)\n"; \
		fi \
    else \
    	printf "$(RED)$(NAME) is already compiled!$(RESET)\n"; \
    fi

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $^ $(LIBFT) -o $(NAME) $(INCLUDES) $(LFLAGS)

cleanlibft:
	@$(MAKE_LIBS) $(LIBFT_DIR) clean

fcleanlibft:
	@$(MAKE_LIBS) $(LIBFT_DIR) fclean

clean:
	@if [ -f $(NAME) ]; then \
		$(MAKE) -s cleanlibft; \
	else \
		printf "$(RED)$(LIBFT_FILE) is not compiled yet!$(RESET)\n"; \
	fi
	@if [ -d $(OBJECTS_DIR) ]; then \
		$(RM) $(OBJECTS_DIR); \
		printf "$(GREEN)Cleaned objects from $(NAME) successfully!$(RESET)\n"; \
	else \
		printf "$(RED)Objects from $(NAME) are already cleaned!$(RESET)\n"; \
	fi

fclean: clean fcleanlibft
	@if [ -f $(NAME) ]; then \
		$(RM) $(NAME); \
		printf "$(GREEN)Removed $(NAME) file successfully!$(RESET)\n"; \
	else \
		printf "$(RED)File $(NAME) is already removed!$(RESET)\n"; \
	fi

re: fclean
	@$(MAKE) -s

bonus: all

$(OBJECTS_DIR)%.o: $(SOURCES_DIR)%.c
	@$(MKDIR) $(OBJECTS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

libraries: $(LIBFT)

$(LIBFT):
	@$(MAKE_LIBS) $(LIBFT_DIR)