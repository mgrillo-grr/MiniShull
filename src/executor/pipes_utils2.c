/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:45:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 12:45:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

int	perror_ret(const char *str, int ret)
{
	perror(str);
	return (ret);
}

int	free_ret(void *ptr, int ret)
{
	free(ptr);
	return (ret);
}

int	count_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	handle_single_command(t_cmd *cmd, t_shell *shell)
{
	if (cmd->redirs && apply_redirections(cmd, shell))
		return (1);
	return (execute_cmd_in_child(cmd, shell));
}

int	wait_for_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < cmd_count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
		{
			perror("minishell: waitpid");
			i++;
			continue ;
		}
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		i++;
	}
	return (last_status);
}
