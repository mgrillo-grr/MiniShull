/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:30:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 16:05:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"
#include "expander.h"
#include "heredoc_utils.h"
#include "heredoc_types.h"

static void	setup_child_process(int pipe_fd[2], char *delimiter_copy,
	t_shell *shell, int expand)
{
	setup_child_signals();
	process_heredoc(pipe_fd, delimiter_copy, shell, expand);
	exit(EXIT_SUCCESS);
}

static int	cleanup_and_return(t_heredoc_data *data, int return_value)
{
	cleanup_heredoc(data->pipe_fd, data->saved_stdin,
		data->saved_stdout, data->delimiter_copy);
	return (return_value);
}

static int	handle_parent_process(pid_t pid, t_heredoc_data *data)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		return (cleanup_and_return(data, -1));
	if (cleanup_heredoc(data->pipe_fd, data->saved_stdin,
			data->saved_stdout, data->delimiter_copy) == -1)
		return (-1);
	return (data->pipe_fd[0]);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	t_heredoc_data	data;
	pid_t			pid;
	int				expand;

	expand = 1;
	data.delimiter_copy = setup_delimiter(delimiter, &expand);
	if (!data.delimiter_copy || !setup_heredoc_pipe(data.pipe_fd,
			&data.saved_stdin, &data.saved_stdout))
	{
		if (data.delimiter_copy)
			free(data.delimiter_copy);
		return (-1);
	}
	setup_heredoc_signals();
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (cleanup_and_return(&data, -1));
	}
	if (pid == 0)
		setup_child_process(data.pipe_fd, data.delimiter_copy, shell, expand);
	return (handle_parent_process(pid, &data));
}
