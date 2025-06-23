/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:50:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 15:50:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_UTILS_H
# define HEREDOC_UTILS_H

# include "minishell.h"

typedef struct s_shell
t_shell;

int		setup_heredoc_pipe(int pipe_fd[2],
		int *saved_stdin, int *saved_stdout);
int		process_heredoc_line(int pipe_fd, char *delimiter,
		t_shell *shell, int expand);
int		cleanup_heredoc(int pipe_fd[2],
		int saved_stdin, int saved_stdout, char *delimiter_copy);
int		process_heredoc(int pipe_fd[2],
		char *delimiter_copy, t_shell *shell, int expand);
char	*setup_delimiter(char *delimiter, int *expand);

#endif
