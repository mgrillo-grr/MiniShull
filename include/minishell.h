/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:26:45 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:26:45 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# define MAX_PATH_LEN 4096
# define HOST_NAME_MAX 64
# define NUM_COLORS 6

/* Mensajes de error */
# define ERR_PREFIX "minishell: "
# define ERR_NOT_FOUND ": command not found"
# define ERR_NO_FILE ": No such file or directory"
# define ERR_NO_PERM ": Permission denied"
# define ERR_NOT_DIR ": Not a directory"
# define ERR_IS_DIR ": Is a directory"
# define ERR_TOO_MANY_ARGS ": too many arguments"
# define ERR_NUM_ARG ": numeric argument required"
# define ERR_SYNTAX ": syntax error near unexpected token `%s'"
# define ERR_HOME ": HOME not set"
# define ERR_OLDPWD ": OLDPWD not set"
# define ERR_PIPE ": pipe error"
# define ERR_FORK ": fork failed"
# define ERR_DUP ": dup2 failed"
# define ERR_MEMORY ": memory allocation failed"
# define ERR_CD_ARGS "cd: too many arguments"
# define ERR_EXIT_ARGS "exit: too many arguments"
# define ERR_EXPORT_ARGS "export: not a valid identifier"
# define ERR_UNSET_ARGS "unset: not a valid identifier"

/* Colores */
# define COLOR_RED "\001\033[1;31m\002"
# define COLOR_GREEN "\001\033[1;32m\002"
# define COLOR_YELLOW "\001\033[1;33m\002"
# define COLOR_BLUE "\001\033[1;34m\002"
# define COLOR_MAGENTA "\001\033[1;35m\002"
# define COLOR_CYAN "\001\033[1;36m\002"
# define COLOR_RESET "\001\033[0m\002"
# define COLOR_GRAY "\001\033[1;90m\002"
# define COLOR_LIGHT_CYAN "\001\033[1;96m\002"

# define PROMPT COLOR_GRAY"╭─"COLOR_LIGHT_CYAN"%s@%s:%s"COLOR_RESET"\n"COLOR_GRAY"╰─"COLOR_RESET"%sminishell"COLOR_RESET"$ "

/* Variable global para manejar señales */
extern int	g_signal_received;

typedef struct s_quote_info
{
	char	*str;
	int		has_single;
	int		has_double;
} t_quote_info;

// Funciones de manejo de quotes
t_quote_info	*remove_quotes(char *str);


// t_env será la cabeza de la lista enlazada
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_redir
{
    int             type;  // 1: <, 2: >, 3: >>, 4: <<
    char            *file;
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char            **args;
    t_redir         *redirs;
    struct s_cmd    *next;
    int             pipe_fd[2];
}   t_cmd;

typedef struct s_shell
{
    t_env   *env;
    t_cmd   *cmd;
    int     exit_status;
    int     in_heredoc;
    int     color_index;
}   t_shell;

// Environment functions
t_env   *init_env(char **envp);
void    free_env(t_env *env);
char    *get_env_value(t_env *env, char *key);
void    set_env_value(t_env *env, char *key, char *value);

// Built-in commands
int	cmd_echo(t_shell *shell, char **args);
int	cmd_cd(t_shell *shell, char **args);
int	cmd_pwd(t_shell *shell, char **args);
int	cmd_export(t_shell *shell, char **args);
int	cmd_unset(t_shell *shell, char **args);
int	cmd_env(t_shell *shell, char **args);
int	cmd_exit(t_shell *shell, char **args);
int	cmd_history(t_shell *shell, char **args);

// Parser functions
t_cmd   *parse_input(char *input, t_shell *shell);
void    free_cmd(t_cmd *cmd);
char    *expand_token(char *token, t_shell *shell);
char    **tokenize(char *input);

// String functions
int     ft_strcmp(const char *s1, const char *s2);
char    **add_token(char **tokens, int *token_count, char *str, int len);
char    *get_next_line(int fd);

// Executor functions
int     execute_cmd(t_shell *shell);
int     execute_builtin(t_cmd *cmd, t_shell *shell);
int     apply_redirections(t_cmd *cmd, t_shell *shell);
int     is_builtin(char *cmd);
int     execute_cmd_in_child(t_cmd *cmd, t_shell *shell);
int     execute_piped_commands(t_cmd *cmd, t_shell *shell);
char    *find_command_path(char *cmd, t_env *env);

// Signal handlers
void    setup_signals(void);
void    reset_signals(void);

// Error handlers
void	print_error_prefix(void);
void	print_error_number(int number);
void	handle_command_not_found(char *cmd, t_shell *shell);
void	handle_unclosed_quotes(t_shell *shell);

// Utils
void    error_msg(char *msg);
char    **env_to_array(t_env *env);
void    free_array(char **array);
int	is_valid_identifier(const char *str);

#endif