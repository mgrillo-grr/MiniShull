/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:35:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 12:35:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

void	close_pipes(t_cmd *cmd)
{
	t_cmd *current = cmd;

	while (current)
	{
		if (current->pipe_fd[0] > 2)
		{
			close(current->pipe_fd[0]);
			current->pipe_fd[0] = -1;
		}

		if (current->pipe_fd[1] > 2)
		{
			close(current->pipe_fd[1]);
			current->pipe_fd[1] = -1;
		}

		current = current->next;
	}
}

int	setup_pipes(t_cmd *cmd)
{
	t_cmd *current = cmd;

	while (current && current->next)
	{
		current->pipe_fd[0] = -1;
		current->pipe_fd[1] = -1;

		if (pipe(current->pipe_fd) == -1)
		{
			perror("minishell: pipe");
			return (1);
		}

		current = current->next;
	}

	if (current)
	{
		current->pipe_fd[0] = -1;
		current->pipe_fd[1] = -1;
	}

	return (0);
}

void	setup_pipe_fds(t_cmd *cmd, t_cmd *prev)
{
	if (prev && prev->pipe_fd[0] > 2)
	{
		if (dup2(prev->pipe_fd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(prev->pipe_fd[0]);
		prev->pipe_fd[0] = -1;
	}

	if (cmd->next && cmd->pipe_fd[1] > 2)
	{
		if (dup2(cmd->pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(cmd->pipe_fd[1]);
		cmd->pipe_fd[1] = -1;
	}

	close_pipes(cmd);
}

int	close_prev_pipes(t_cmd *prev)
{
	if (close(prev->pipe_fd[0]) == -1)
	{
		perror("minishell: close");
		return (1);
	}
	if (close(prev->pipe_fd[1]) == -1)
	{
		perror("minishell: close");
		return (1);
	}
	return (0);
}
