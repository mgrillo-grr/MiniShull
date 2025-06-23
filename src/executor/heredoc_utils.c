/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:50:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 17:00:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"
#include "heredoc_utils.h"

int	setup_heredoc_pipe(int pipe_fd[2],
	int *saved_stdin, int *saved_stdout)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("minishell: dup");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	return (1);
}

int	process_heredoc(int pipe_fd[2], char *delimiter_copy,
	t_shell *shell, int expand)
{
	shell->in_heredoc = 1;
	g_signal_received = 0;
	while (1)
	{
		if (!process_heredoc_line(pipe_fd[1], delimiter_copy, shell, expand))
			break ;
	}
	shell->in_heredoc = 0;
	return (0);
}

char	*setup_delimiter(char *delimiter, int *expand)
{
	char	*delimiter_copy;
	char	*tmp;
	size_t	len;

	delimiter_copy = ft_strdup(delimiter);
	if (!delimiter_copy)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	if (delimiter_copy[0] == '\'' || delimiter_copy[0] == '"')
	{
		len = ft_strlen(delimiter_copy);
		if (delimiter_copy[len - 1] == delimiter_copy[0])
		{
			*expand = 0;
			tmp = delimiter_copy;
			delimiter_copy = ft_substr(delimiter_copy, 1, len - 2);
			free(tmp);
		}
	}
	return (delimiter_copy);
}
