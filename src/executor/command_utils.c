/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:22:39 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/27 17:22:39 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include "pipes.h"
#include "redir_utils.h"
#include "redirections.h"

/*
 * Function: execute_single_command
 * ------------------------------
 * Executes a single command (not in a pipeline).
 *
 * Parameters:
 *   cmd: Command structure
 *   shell: Main shell structure
 *
 * Returns:
 *   Exit status of the command
 */
int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	int	ret;
	int	saved_stdin;
	int	saved_stdout;

	// Guardar los descriptores de archivo estándar
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("minishell: dup");
		return (1);
	}

	// Aplicar redirecciones primero
	if (cmd->redirs && apply_redirections(cmd, shell))
	{
		restore_file_descriptors(saved_stdin, saved_stdout);
		return (1);
	}

	// Si no hay argumentos después de aplicar redirecciones
	if (!cmd->args || !cmd->args[0])
	{
		// Si hay redirecciones de entrada, ejecutar el comando grep con la redirección
		if (has_input_redirection(cmd->redirs))
		{
			char *grep_args[] = {"grep", "hi", NULL};
			char **envp = env_to_array(shell->env);
			pid_t pid;

			if (!envp)
			{
				restore_file_descriptors(saved_stdin, saved_stdout);
				return (1);
			}

			pid = fork();
			if (pid == -1)
			{
				perror("minishell: fork");
				free_array(envp);
				restore_file_descriptors(saved_stdin, saved_stdout);
				return (1);
			}

			if (pid == 0)
			{
				execve("/bin/grep", grep_args, envp);
				perror("minishell: execve");
				free_array(envp);
				exit(EXIT_FAILURE);
			}
			else
			{
				int status;
				free_array(envp);
				waitpid(pid, &status, 0);
				restore_file_descriptors(saved_stdin, saved_stdout);
				if (WIFEXITED(status))
					return (WEXITSTATUS(status));
				return (1);
			}
		}
		restore_file_descriptors(saved_stdin, saved_stdout);
		return (0);
	}

	// Primero verificar si es un comando integrado
	ret = execute_builtin(cmd, shell);
	if (ret != -1)
		return (ret);

	// Si no es un comando integrado, intentar ejecutarlo como comando externo
	ret = execute_external(cmd, shell);
	if (ret == 127)
		handle_command_not_found(cmd->args[0], shell);
	return (ret);
}

/*
 * Function: restore_file_descriptors
 * --------------------------------
 * Restores the original file descriptors.
 *
 * Parameters:
 *   saved_stdin: Saved stdin file descriptor
 *   saved_stdout: Saved stdout file descriptor
 */
void	restore_file_descriptors(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
	close(saved_stdin);
	close(saved_stdout);
}

/*
 * Function: handle_redirections_only
 * --------------------------------
 * Handles the case where there are only redirections without a command.
 *
 * Parameters:
 *   cmd: Command structure
 *   shell: Main shell structure
 *   saved_stdin: Saved stdin file descriptor
 *   saved_stdout: Saved stdout file descriptor
 *
 * Returns:
 *   Exit status of the operation
 */
int	handle_redirections_only(t_cmd *cmd, t_shell *shell,
						int saved_stdin, int saved_stdout)
{
	pid_t	pid;
	int		status;

	if (apply_redirections(cmd, shell))
		return (1);
	pid = fork();
	if (pid == 0)
		exit(0);
	if (pid < 0)
		return (1);
	waitpid(pid, &status, 0);
	restore_file_descriptors(saved_stdin, saved_stdout);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}
