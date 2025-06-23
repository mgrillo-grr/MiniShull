/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:21:20 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 12:10:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

/*
 * Function: has_input_redirection
 * -----------------------------
 * Checks if there's any input redirection in the command.
 *
 * Parameters:
 *   redirs: List of redirections
 *
 * Returns:
 *   1 if there's an input redirection, 0 otherwise
 */
static int	has_input_redirection(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == 1 || current->type == 4) // Input or heredoc
			return (1);
		current = current->next;
	}
	return (0);
}

/*
 * Function: execute_cmd
 * ------------------
 * Executes a command, either builtin or external.
 *
 * Parameters:
 *   shell: Main shell structure
 *
 * Process:
 * 1. Checks if there's a command to execute
 * 2. If it's a builtin, executes it directly
 * 3. If it's not a builtin, executes it as an external command
 *
 * Returns:
 *   Exit code of the executed command
 */
static int	handle_dup_errors(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
		close(saved_stdin);
	if (saved_stdout != -1)
		close(saved_stdout);
	return (1);
}

static int	handle_redirections(t_shell *shell,
				int saved_stdin, int saved_stdout)
{
	if (shell->cmd->redirs)
	{
		if (apply_redirections(shell->cmd, shell))
		{
			restore_file_descriptors(saved_stdin, saved_stdout);
			return (1);
		}
		// If there's no command, just process the redirections
		if (!shell->cmd->args || !shell->cmd->args[0])
		{
			// For input redirections, we need to read from the file
			// but don't execute any command
			if (has_input_redirection(shell->cmd->redirs))
			{
				// Just read from stdin (which is now the file)
				char buffer[1024];
				while (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0)
					;
			}
			restore_file_descriptors(saved_stdin, saved_stdout);
			return (0);
		}
	}
	return (-1);
}

static int	execute_command_logic(t_shell *shell)
{
	// If there are redirections but no command, just process the redirections and exit
	if (shell->cmd->redirs && (!shell->cmd->args || !shell->cmd->args[0]))
	{
		// Apply redirections and exit with success
		if (apply_redirections(shell->cmd, shell) == 0)
			return (0);
		return (1);
	}
	if (!shell->cmd->next)
		return (execute_single_command(shell->cmd, shell));
	return (execute_piped_commands(shell->cmd, shell));
}

int	execute_cmd(t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	if (!shell || !shell->cmd)
		return (1);
	saved_stdin = dup(0);
	saved_stdout = dup(1);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (handle_dup_errors(saved_stdin, saved_stdout));
	ret = handle_redirections(shell, saved_stdin, saved_stdout);
	if (ret != -1)
		return (ret);
	ret = execute_command_logic(shell);
	restore_file_descriptors(saved_stdin, saved_stdout);
	return (ret);
}
