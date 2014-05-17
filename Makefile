#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sconso <sconso@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/01/12 18:13:08 by sconso            #+#    #+#              #
#    Updated: 2014/05/17 18:27:41 by sconso           ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = wolf3d

CC = cc
CFLAGS = -Wall -Werror -Wextra
DFLAGS = -pedantic -g -ggdb
INC = -I includes/ -I libft/includes/ -I /usr/X11/include
LFLAGS = -L libft/ -lft -L/usr/lib -lmlx -L/usr/X11/lib -lXext -lX11

FILES = draw.c draw_utilities.c ft_errors.c ft_file.c ft_wolf.c ft_matrix.c \
		keys.c

SRCDIR = src
OBJDIR = obj

SRC = $(addprefix $(SRCDIR)/, $(FILES))
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all : $(NAME)

$(NAME) : libft $(OBJDIR) $(OBJ)
		@make -C libft/ all
		@$(CC) $(CFLAGS) $(INC) -o $@ $(OBJ) $(LFLAGS)
		@echo "\033[4;32m$@\033[0m created !"

$(OBJDIR) :
		@/bin/mkdir $(OBJDIR);			\
		for DIR in $(SRCDIR);			\
		do								\
			/bin/mkdir $(OBJDIR)/$$DIR;	\
		done							\

$(addprefix $(OBJDIR)/, %.o) : %.c
	@$(CC) $(CFLAGS) $(INC) -o $@ -c $<
	@/bin/echo -n "Creating objects in $(addprefix $(OBJDIR)/, $(SRCDIR)) : "
	@echo "\033[1;33m$(subst $(SRCDIR)/, , $(<:.c=.o))\033[0m"

run : re
		@/usr/bin/clear
		@./$(NAME)

clean :
		@/bin/rm -rf $(OBJDIR) a.out.dSYM
		@/bin/rm -f a.out
		@echo "\033[1;30m$(OBJDIR)\033[0m removed !"

fclean : clean
		@/bin/rm -f $(NAME)
		@echo "\033[1;30m$(NAME)\033[0m removed !"

libft :
		@make -C libft all

libfclean :
		@make -C libft fclean

libre :
		@make -C libft re
		@make re

re : fclean all

debug : CFLAGS += $(DFLAGS)
debug : re

.PHONY : all clean fclean re
