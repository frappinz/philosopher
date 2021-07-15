#Name
NAME =			philo

#Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

#Norm
NORM =			norminette

#Shell
MKDIR =			mkdir -p
RM =			rm -rf

#Directories
SRCDIR = 		./
OBJDIR =  		./
INCDIR = 		./

FILES =			philo.c		\
				ft_atoi.c	\

#Srcs
SRCS = 			$(foreach FILE, $(FILES), $(shell find $(SRCDIR) -name $(FILE)))

#Objs
OBJS = 			$(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRCS:.c=.o))

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) -I $(INCDIR) -c -o $@ $<

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@clear -c

norminette:
	@$(NORM)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

run:
	@clear -c
	@make clean --quiet
	@./philo

re: fclean all

.PHONY: all norminette sanitize libft clean fclean run re