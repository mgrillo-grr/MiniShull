NAME = libft.a

CC	= cc

FLAGS = -Wall -Wextra -Werror -I ../include 
FLAGS_GNL = -Wall -Wextra -Werror -I ../../include 

AR = ar rcs

RM	= rm -rf

FILES = ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
	ft_isprint.c ft_memset.c ft_strchr.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
	ft_strncmp.c ft_strrchr.c ft_bzero.c ft_memchr.c ft_memcmp.c ft_memmove.c \
	ft_memcpy.c ft_strnstr.c ft_tolower.c ft_toupper.c ft_calloc.c ft_strdup.c \
	ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_putchar_fd.c \
	ft_strmapi.c ft_striteri.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
	ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstadd_back_bonus.c \
	ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c \
	ft_lstmap_bonus.c

GNL = ./get_next_line/get_next_line.c ./get_next_line/get_next_line_utils.c \

OBJS		= $(FILES:.c=.o)
OBJS_GNL	= $(GNL:.c=.o)

%.o:%.c
	$(CC) $(FLAGS) -c $< -o ${<:.c=.o}

$(NAME):	$(OBJS) $(OBJS_GNL)
		$(AR) $(NAME) $(OBJS) $(OBJS_GNL)

all:	${NAME}

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_GNL)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(OBJS_GNL)

re: fclean all

.PHONY:	all clean fclean re

.SILENT:
