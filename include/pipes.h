/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:40:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 14:55:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include "minishell.h"

// Forward declaration para t_quote_info
typedef struct s_quote_info t_quote_info;

typedef struct s_child_data
{
	t_cmd	*cmd;
	t_shell	*shell;
	t_cmd	*prev;
	pid_t	*pids;
	int		i;
} t_child_data;

// pipes.c
int		execute_piped_commands(t_cmd *cmd, t_shell *shell);
int		execute_cmd_in_child(t_cmd *cmd, t_shell *shell);
int		create_child_process(t_child_data data);

// pipes_utils.c
void	close_pipes(t_cmd *cmd);
int		setup_pipes(t_cmd *cmd);
void	setup_pipe_fds(t_cmd *cmd, t_cmd *prev);
int		close_prev_pipes(t_cmd *prev);

// pipes_utils2.c
int		perror_ret(const char *str, int ret);
int		free_ret(void *ptr, int ret);
int		count_commands(t_cmd *cmd);
int		handle_single_command(t_cmd *cmd, t_shell *shell);
int		wait_for_children(pid_t *pids, int cmd_count);

// pipes_utils3.c
int		handle_external_command(t_cmd *cmd, t_shell *shell, char *cmd_path);
int		execute_external_command(t_cmd *cmd, t_shell *shell, t_quote_info *info);

// pipes_utils4.c
int		handle_child_execution(t_cmd *cmd, t_shell *shell, t_cmd *prev);
int		handle_fork_error(pid_t *pids, int i);

#endif
