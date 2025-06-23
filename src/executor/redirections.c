/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:00:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 15:45:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirections.h"
#include "expander.h"
#include "redir_utils.h"

static int	handle_redirection(t_redir *redir, t_shell *shell, int *error)
{
	if (redir->type == 1 || redir->type == 4)
	{
		if (!handle_input_redir(redir, shell, error))
			return (0);
	}
	else if (redir->type == 2 || redir->type == 3)
	{
		if (!handle_output_redir(redir, error))
			return (0);
	}
	return (1);
}

static int	setup_redirections(t_cmd *cmd, t_shell *shell, int *error)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir && !*error)
	{
		handle_redirection(redir, shell, error);
		redir = redir->next;
	}
	return (*error);
}

int	has_input_redirection(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == 1 || redir->type == 4) // Input or here-doc
			return (1);
		redir = redir->next;
	}
	return (0);
}

int	apply_redirections(t_cmd *cmd, t_shell *shell)
{
	static int	saved_stdin = -1;
	static int	saved_stdout = -1;
	int			error;

	// Si ya hay redirecciones aplicadas, restaurar los descriptores originales
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		saved_stdin = -1;
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		saved_stdout = -1;
	}

	// Si no hay redirecciones que aplicar, salir
	if (!cmd->redirs)
		return (0);

	// Guardar los descriptores originales
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("minishell: dup");
		return (1);
	}

	// Aplicar las redirecciones
	error = 0;
	setup_redirections(cmd, shell, &error);

	return (error);
}
