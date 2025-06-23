/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:45:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 14:45:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

int	handle_child_execution(t_cmd *cmd, t_shell *shell, t_cmd *prev)
{
	if (cmd->redirs && apply_redirections(cmd, shell))
		exit(1);

	setup_pipe_fds(cmd, prev);
	close_pipes(cmd);

	if (cmd->pipe_fd[0] > 2)
	{
		close(cmd->pipe_fd[0]);
		cmd->pipe_fd[0] = -1;
	}

	if (prev && prev->pipe_fd[1] > 2)
	{
		close(prev->pipe_fd[1]);
		prev->pipe_fd[1] = -1;
	}

	exit(execute_cmd_in_child(cmd, shell));
}

int	handle_fork_error(pid_t *pids, int i)
{
	perror("minishell: fork");
	while (--i >= 0)
		kill(pids[i], SIGTERM);
	free(pids);
	return (1);
}

int	create_child_process(t_child_data data)
{
	data.pids[data.i] = fork();
	if (data.pids[data.i] == -1)
		return (handle_fork_error(data.pids, data.i));
	if (data.pids[data.i] == 0)
	{
		free(data.pids);
		handle_child_execution(data.cmd, data.shell, data.prev);
	}
	return (0);
}
