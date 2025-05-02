/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_input_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd(filename, 2);
		ft_putendl_fd(ERR_NO_FILE, 2);
		return (-1);
	}
	return (fd);
}

static int	open_output_file(char *filename, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;

	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd(filename, 2);
		ft_putendl_fd(ERR_NO_PERM, 2);
		return (-1);
	}
	return (fd);
}

static int	handle_heredoc(char *delimiter, t_shell *shell)
{
	int	pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (-1);

	shell->in_heredoc = 1;
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line || (ft_strlen(line) - 1 == ft_strlen(delimiter) &&
			ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0))
		{
			free(line);
			break;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
	}
	shell->in_heredoc = 0;
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	apply_redirections(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == 1) // <
		{
			fd = open_input_file(redir->file);
			if (fd == -1)
				return (1);
			dup2(fd, 0);
			close(fd);
		}
		else if (redir->type == 2) // >
		{
			fd = open_output_file(redir->file, 0);
			if (fd == -1)
				return (1);
			dup2(fd, 1);
			close(fd);
		}
		else if (redir->type == 3) // >>
		{
			fd = open_output_file(redir->file, 1);
			if (fd == -1)
				return (1);
			dup2(fd, 1);
			close(fd);
		}
		else if (redir->type == 4) // <<
		{
			fd = handle_heredoc(redir->file, shell);
			if (fd == -1)
				return (1);
			dup2(fd, 0);
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}
