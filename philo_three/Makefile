NAME = philosophers

CC = gcc

CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c				\
	  philosopher.c			\
	  philosopher_util.c	\
	  philosopher_func.c

OBJS = $(SRCS:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -f $(OBJS)
	@echo clean up!!

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
