/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:56:14 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 12:00:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_env
t_env;

typedef struct s_cmd
t_cmd;

typedef struct s_shell
t_shell;

/* command_path.c */
char	*find_command_path(char *cmd, t_env *env);

/* command_execution.c */
int		execute_external(t_cmd *cmd, t_shell *shell);
int		execute_piped_commands(t_cmd *cmd, t_shell *shell);

/* command_utils.c */
int		execute_single_command(t_cmd *cmd, t_shell *shell);
void	restore_file_descriptors(int saved_stdin, int saved_stdout);
int		handle_redirections_only(t_cmd *cmd, t_shell *shell,
						int saved_stdin, int saved_stdout);

#endif
