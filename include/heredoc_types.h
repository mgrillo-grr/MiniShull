/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_types.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:00:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 16:00:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_TYPES_H
# define HEREDOC_TYPES_H

typedef struct s_heredoc_data
{
	int		pipe_fd[2];
	int		saved_stdin;
	int		saved_stdout;
	char	*delimiter_copy;
} t_heredoc_data;

#endif
