NAME = libftprintf.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -I ../../include
RM = rm -f
AR = ar rcs
SRCS = ft_printf.c ft_printf_aux.c ft_printf_aux1.c
OBJS = $(SRCS:.c=.o)


all:$(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all, clean, fclean

.SILENT: