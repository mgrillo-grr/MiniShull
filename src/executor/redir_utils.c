/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:30:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 15:40:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"
#include "redir_utils.h"
#include <string.h>

/*
 * Function: remove_quotes_from_filename
 * -------------------------------------
 * Removes surrounding quotes from a filename if they exist.
 *
 * Parameters:
 *   filename: The filename to process
 *
 * Returns:
 *   A new string with quotes removed, or NULL on error
 */
static char	*remove_quotes_from_filename(const char *filename)
{
	char	*result;
	int		len;

	if (!filename)
		return (NULL);

	len = ft_strlen(filename);
	if (len >= 2 && 
		((filename[0] == '"' && filename[len-1] == '"') ||
		 (filename[0] == '\'' && filename[len-1] == '\'')))
	{
		result = ft_substr(filename, 1, len - 2);
	}
	else
	{
		result = ft_strdup(filename);
	}

	return (result);
}

int	open_input_file(char *filename)
{
	int	fd;
	char	*unquoted;

	// Remove surrounding quotes if they exist
	unquoted = remove_quotes_from_filename(filename);
	if (!unquoted)
		return (-1);

	fd = open(unquoted, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(unquoted, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		free(unquoted);
		return (-1);
	}
	free(unquoted);
	return (fd);
}

int	open_output_file(char *filename, int append)
{
	int	fd;
	char	*unquoted;

	// Remove surrounding quotes if they exist
	unquoted = remove_quotes_from_filename(filename);
	if (!unquoted)
		return (-1);

	if (append)
		fd = open(unquoted, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(unquoted, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(unquoted, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		free(unquoted);
		return (-1);
	}
	free(unquoted);
	return (fd);
}

int	handle_input_redir(t_redir *redir, t_shell *shell, int *error)
{
	int	fd;

	if (redir->type == 1)
		fd = open_input_file(redir->file);
	else
		fd = handle_heredoc(redir->file, shell);
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		if (fd != -1)
			close(fd);
		*error = 1;
		return (0);
	}
	close(fd);
	return (1);
}

int	handle_output_redir(t_redir *redir, int *error)
{
	int	fd;

	fd = open_output_file(redir->file, redir->type == 3);
	if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
	{
		if (fd != -1)
			close(fd);
		*error = 1;
		return (0);
	}
	close(fd);
	return (1);
}

// Esta función ya no es necesaria, ya que la limpieza se maneja en apply_redirections
// Se mantiene por compatibilidad pero no hace nada
void	cleanup_redirections(int saved_stdin, int saved_stdout)
{
	(void)saved_stdin;
	(void)saved_stdout;
}
