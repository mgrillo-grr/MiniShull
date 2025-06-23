/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:35:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 15:40:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_UTILS_H
# define REDIR_UTILS_H

# include "minishell.h"

typedef struct s_redir
t_redir;

typedef struct s_shell
t_shell;

int		open_input_file(char *filename);
int		open_output_file(char *filename, int append);
int		handle_heredoc(char *delimiter, t_shell *shell);
int		handle_input_redir(t_redir *redir, t_shell *shell, int *error);
int		handle_output_redir(t_redir *redir, int *error);
void	cleanup_redirections(int saved_stdin, int saved_stdout);

#endif
