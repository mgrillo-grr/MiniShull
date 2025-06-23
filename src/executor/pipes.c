/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 14:45:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

static int	execute_pipeline_loop(t_child_data *data, int cmd_count)
{
	t_cmd	*current;
	int		i;

	current = data->cmd;
	i = 0;
	while (current && i < cmd_count)
	{
		data->cmd = current;
		data->i = i++;
		if (create_child_process(*data) || (data->prev
				&& close_prev_pipes(data->prev)))
			return (1);
		data->prev = current;
		current = current->next;
	}
	close_pipes(data->cmd);
	return (0);
}

static int	execute_pipeline(t_cmd *cmd, t_shell *shell, int cmd_count)
{
	t_child_data	data;
	int			status;

	data.pids = malloc(sizeof(pid_t) * cmd_count);
	if (!data.pids)
		return (perror_ret("minishell: malloc", 1));
	if (setup_pipes(cmd))
		return (free_ret(data.pids, 1));
	data.cmd = cmd;
	data.shell = shell;
	data.prev = NULL;
	if (execute_pipeline_loop(&data, cmd_count))
	{
		free(data.pids);
		return (1);
	}
	status = wait_for_children(data.pids, cmd_count);
	free(data.pids);
	return (status);
}

int	execute_piped_commands(t_cmd *cmd, t_shell *shell)
{
	int	cmd_count;

	cmd_count = count_commands(cmd);
	if (cmd_count == 1)
		return (handle_single_command(cmd, shell));
	return (execute_pipeline(cmd, shell, cmd_count));
}
