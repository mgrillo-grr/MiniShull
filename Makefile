NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I ./include #-g3 -fsanitize=address 
LDFLAGS = -lreadline -fsanitize=address
AR = ar rcs
RM = rm -rf

GREEN := \033[1;32m
RED := \033[1;31m
RESET := \033[0m

# Source files
SRCS = 	./src/main.c \
		./src/builtins/cmd_echo.c \
		./src/builtins/cmd_cd.c \
		./src/builtins/cmd_pwd.c \
		./src/builtins/cmd_export.c \
		./src/builtins/cmd_unset.c \
		./src/builtins/cmd_env.c \
		./src/builtins/cmd_exit.c \
		./src/builtins/history.c \
		./src/parser/parser.c \
		./src/parser/lexer.c \
		./src/parser/expander.c \
		./src/executor/executor.c \
		./src/executor/redirections.c \
		./src/executor/redir_utils.c \
		./src/executor/heredoc.c \
		./src/executor/heredoc_utils.c \
		./src/executor/heredoc_utils2.c \
		./src/executor/pipes.c \
		./src/executor/pipes_utils.c \
		./src/executor/pipes_utils2.c \
		./src/executor/pipes_utils3.c \
		./src/executor/pipes_utils4.c \
		./src/executor/builtins.c \
		./src/executor/builtin_utils.c \
		./src/executor/command_path.c \
		./src/executor/command_execution.c \
		./src/executor/command_utils.c \
		./src/env/env_utils.c \
		./src/env/env_array.c \
		./src/signals/signals.c \
		./src/utils/utils.c \
		./src/errors/error_handlers.c

# Object files
OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Create object directories
OBJ_DIRS = $(sort $(dir $(OBJS)))

# Libft directory
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

MAKEFLAGS = --no-print-directory

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "[100%] $(GREEN)Compilation successful!$(RESET)"

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) $(OBJ_DIR)
	@echo "[100%] $(RED)Cleaning object files...$(RESET)"

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) $(NAME)
	@echo "[100%] $(RED)Cleaning everything...$(RESET)"

re: fclean all

norm:
	norminette $(shell find ./src ./include -name "*.c" -o -name "*.h")

.PHONY: all clean fclean re norm

.SILENT: