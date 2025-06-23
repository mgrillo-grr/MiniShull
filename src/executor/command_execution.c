/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:22:39 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 12:30:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>    // Para isatty()
#include <stdlib.h>    // Para free()
#include <string.h>    // Para strcmp()

static char	**process_args_array(char **args, int count);

/*
 * Processes command arguments by removing quotes.
 *
 * @param   args    Array of command arguments
 * @return  New array with processed arguments, NULL on error
*/
static char	**process_command_arguments(char **args)
{
	int	count;

	if (!args || !*args)
		return (NULL);
	count = 0;
	while (args[count])
		count++;
	return (process_args_array(args, count));
}

/*
 * Processes an array of arguments, removing quotes from each one.
 *
 * @param   args    Array of arguments to process
 * @param   count   Number of arguments in the array
 * @return  New array with processed arguments, NULL on error
 */
static char	**process_args_array(char **args, int count)
{
	char			**processed;
	t_quote_info	*info;
	int				i;

	processed = malloc(sizeof(char *) * (count + 1));
	if (!processed)
		return (NULL);
	i = 0;
	while (i < count)
	{
		info = remove_quotes(args[i]);
		if (!info)
		{
			free_array(processed);
			return (NULL);
		}
		processed[i] = info->str;
		free(info);
		i++;
	}
	processed[i] = NULL;
	return (processed);
}

/*
 * Handles the child process execution of an external command.
 *
 * @param   cmd       Command structure
 * @param   shell     Main shell structure
 * @param   cmd_path  Full path to the command
 * @param   args      Processed command arguments
 * @note    This function does not return, it exits the process.
 */
static void	execute_child_process(t_cmd *cmd, t_shell *shell,
			char *cmd_path, char **args)
{
	char	**envp;

	// Configurar las señales para el proceso hijo
	setup_child_signals();

	// Aplicar redirecciones primero
	if (cmd->redirs && apply_redirections(cmd, shell))
		exit(1);

	// Ejecutar el comando
	envp = env_to_array(shell->env);
	if (!envp)
	{
		perror("minishell: env_to_array");
		exit(1);
	}

	execve(cmd_path, args, envp);
	
	// Si llegamos aquí, hubo un error
	perror("minishell");
	free_array(envp);
	if (args != cmd->args) // Si modificamos los argumentos
		free(args);
	exit(126);
}

/*
 * Executes an external command.
 *
 * @param   cmd     Command structure to execute
 * @param   shell   Main shell structure
 * @return  Command exit code, 127 if command not found, 126 if permission denied
 */
static int	wait_for_child(pid_t pid, char *cmd_path, char **args)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free_array(args);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;
	char	**processed_args;
	pid_t	pid;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);

	// Check if the command is a path to an executable
	if (access(cmd->args[0], X_OK) == 0)
		cmd_path = ft_strdup(cmd->args[0]);
	else
		cmd_path = find_command_path(cmd->args[0], shell->env);

	if (!cmd_path)
	{
		shell->exit_status = 127;
		return (127);
	}

	processed_args = process_command_arguments(cmd->args);
	if (!processed_args)
	{
		free(cmd_path);
		return (1);
	}

	pid = fork();
	if (pid == 0)
		execute_child_process(cmd, shell, cmd_path, processed_args);

	// In the parent process
	return (wait_for_child(pid, cmd_path, processed_args));
}
