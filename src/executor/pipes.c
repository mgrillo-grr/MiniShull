/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipes(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->pipe_fd[0] > 2)
			close(cmd->pipe_fd[0]);
		if (cmd->pipe_fd[1] > 2)
			close(cmd->pipe_fd[1]);
		cmd = cmd->next;
	}
}

static int	setup_pipes(t_cmd *cmd)
{
	while (cmd && cmd->next)
	{
		cmd->pipe_fd[0] = -1;
		cmd->pipe_fd[1] = -1;
		if (pipe(cmd->pipe_fd) == -1)
		{
			perror("minishell");
			return (1);
		}
		cmd = cmd->next;
	}
	if (cmd)
	{
		cmd->pipe_fd[0] = -1;
		cmd->pipe_fd[1] = -1;
	}
	return (0);
}

static void	setup_pipe_fds(t_cmd *cmd, t_cmd *prev)
{
	if (prev && prev->pipe_fd[0] > 2)
	{
		dup2(prev->pipe_fd[0], 0);
		close(prev->pipe_fd[0]);
	}
	if (cmd->next && cmd->pipe_fd[1] > 2)
	{
		dup2(cmd->pipe_fd[1], 1);
		close(cmd->pipe_fd[1]);
	}
}

int	execute_piped_commands(t_cmd *cmd, t_shell *shell)
{
	pid_t	*pids;
	t_cmd	*current;
	t_cmd	*prev;
	int		status;
	int		last_status;
	int		cmd_count;
	int		i;

	cmd_count = 0;
	current = cmd;
	while (current)
	{
		cmd_count++;
		current = current->next;
	}

	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (1);

	if (setup_pipes(cmd))
	{
		free(pids);
		return (1);
	}

	current = cmd;
	prev = NULL;
	last_status = 0;
	i = 0;

	while (current && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			free(pids);
			if (current->redirs && apply_redirections(current, shell))
				exit(1);
			setup_pipe_fds(current, prev);
			close_pipes(cmd);
			exit(execute_cmd_in_child(current, shell));
		}
		if (prev)
		{
			close(prev->pipe_fd[0]);
			close(prev->pipe_fd[1]);
		}
		prev = current;
		current = current->next;
		i++;
	}

	close_pipes(cmd);

	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
	}

	free(pids);
	return (last_status);
}

int	execute_cmd_in_child(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;
	char	**envp;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(1);

	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));

	cmd_path = find_command_path(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}

	envp = env_to_array(shell->env);
	if (!envp)
	{
		free(cmd_path);
		exit(1);
	}

	execve(cmd_path, cmd->args, envp);
	perror("minishell");
	free(cmd_path);
	free_array(envp);
	exit(126);
}
