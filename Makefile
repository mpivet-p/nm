# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpivet-p <mpivet-p@42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/22 02:45:47 by mpivet-p          #+#    #+#              #
#    Updated: 2021/07/31 14:48:56 by mpivet-p         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= ft_nm
CC= gcc
CFLAGS= -Wall -Wextra -Werror -std=c99 -g #-fsanitize=address

INC_PATH= includes/
SRC_PATH= srcs/
OBJ_PATH= objs/

INC_NAME= ft_nm.h
SRC_NAME= main.c fill_header.c get_section_headers.c get_header.c\
			protect.c get_symbols.c symbol_list.c get_file.c\
			fill_section_header.c nm.c
OBJ_NAME= $(SRC_NAME:.c=.o)

INC= -I $(INC_PATH) -I libft/inc/
SRC= $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ= $(addprefix $(OBJ_PATH), $(OBJ_NAME))
LIBS= -L libft/ -lft

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft --no-print-directory
	gcc $(CFLAGS) -o $(NAME) $(OBJ) $(INC) $(LIBS)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

32bits:
	$(CC) -m32 -ggdb3 -O0 -pedantic-errors -std=c89 -Wall -Wextra -pedantic -o 32bits 32bits.c

clean:
	@make -C libft clean --no-print-directory
	rm -rf $(OBJ_PATH) 32bits

fclean: clean
	@make -C libft fclean --no-print-directory
	rm -rf $(NAME)

re: fclean all
