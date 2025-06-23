/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:50:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 16:50:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"
#include "heredoc_utils.h"

static int	expand_and_write_line(int pipe_fd, char *line,
	t_shell *shell, int expand)
{
	char	*expanded_line;
	int		result;

	expanded_line = line;
	if (expand)
		expanded_line = expand_env_vars(line, shell->env);
	if (!expanded_line)
	{
		free(line);
		return (0);
	}
	result = 1;
	if (write(pipe_fd, expanded_line, ft_strlen(expanded_line)) == -1
		|| write(pipe_fd, "\n", 1) == -1)
	{
		perror("minishell: write");
		result = 0;
	}
	if (expand && expanded_line != line)
		free(expanded_line);
	free(line);
	return (result);
}

int	process_heredoc_line(int pipe_fd, char *delimiter,
	t_shell *shell, int expand)
{
	char	*line;
	char	*trimmed;
	int		len;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	if (!line || g_signal_received)
	{
		free(line);
		return (0);
	}
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[--len] = '\0';
	trimmed = ft_strtrim(line, " \t");
	if (!trimmed || ft_strcmp(trimmed, delimiter) == 0)
	{
		free(trimmed);
		free(line);
		return (0);
	}
	free(trimmed);
	return (expand_and_write_line(pipe_fd, line, shell, expand));
}

int	cleanup_heredoc(int pipe_fd[2], int saved_stdin,
	int saved_stdout, char *delimiter_copy)
{
	int	result;

	result = 0;
	if (dup2(saved_stdin, STDIN_FILENO) == -1
		|| dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("minishell: dup2");
	close(saved_stdin);
	close(saved_stdout);
	if (close(pipe_fd[1]) == -1)
	{
		perror("minishell: close");
		result = -1;
	}
	free(delimiter_copy);
	return (result);
}
