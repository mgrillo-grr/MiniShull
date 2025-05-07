/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: find_command_path
 * ----------------------
 * Busca la ruta completa de un comando en el PATH.
 *
 * Parámetros:
 *   cmd: Nombre del comando
 *   env: Variables de entorno
 *
 * Retorna:
 *   La ruta completa del comando si se encuentra
 *   NULL si no se encuentra
 */
char	*find_command_path(char *cmd, t_env *env)
{
	char	*path;
	char	**paths;
	char	*full_path;
	int		i;
	struct stat	st;
	t_quote_info *info;
	char	*processed_cmd;

	if (!cmd || !*cmd)
		return (NULL);

	// Procesar las comillas del comando
	info = remove_quotes(cmd);
	if (!info)
		return (NULL);
	processed_cmd = info->str;

	if (processed_cmd[0] == '/' || processed_cmd[0] == '.')
	{
		if (stat(processed_cmd, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(info);
			return (ft_strdup(processed_cmd));
		}
		free(info->str);
		free(info);
		return (NULL);
	}

	path = get_env_value(env, "PATH");
	if (!path)
	{
		free(info->str);
		free(info);
		return (NULL);
	}

	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(ft_strjoin(paths[i], "/"), processed_cmd);
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(paths);
			free(info->str);
			free(info);
			return (full_path);
		}
		free(full_path);
		i++;
	}

	free(paths);
	free(info->str);
	free(info);
	return (NULL);
}

/*
 * Función: execute_external
 * -----------------------
 * Ejecuta un comando externo.
 *
 * Parámetros:
 *   cmd: Estructura del comando a ejecutar
 *   shell: Estructura principal del shell
 *
 * Retorna:
 *   El código de salida del comando
 *   127 si el comando no se encuentra
 *   126 si hay un error de permisos
 */
static int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;
	char	**envp;
	pid_t	pid;
	int		status;
	int		i;
	char	**processed_args;
	t_quote_info	*info;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);

	cmd_path = find_command_path(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		shell->exit_status = 127;
		return (127);
	}

	// Procesar las comillas de los argumentos
	i = 0;
	while (cmd->args[i])
		i++;
	processed_args = malloc(sizeof(char *) * (i + 1));
	if (!processed_args)
	{
		free(cmd_path);
		return (1);
	}

	i = 0;
	while (cmd->args[i])
	{
		info = remove_quotes(cmd->args[i]);
		if (!info)
		{
			free(cmd_path);
			free_array(processed_args);
			return (1);
		}
		processed_args[i] = info->str;
		free(info);
		i++;
	}
	processed_args[i] = NULL;

	pid = fork();
	if (pid == 0)
	{
		if (cmd->redirs && apply_redirections(cmd, shell))
			exit(1);
		envp = env_to_array(shell->env);
		execve(cmd_path, processed_args, envp);
		perror("minishell");
		exit(126);
	}
	free(cmd_path);
	free_array(processed_args);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/*
 * Función: execute_cmd
 * ------------------
 * Ejecuta un comando, ya sea builtin o externo.
 *
 * Parámetros:
 *   shell: Estructura principal del shell
 *
 * Proceso:
 * 1. Verifica si hay un comando para ejecutar
 * 2. Si es un builtin, lo ejecuta directamente
 * 3. Si no es builtin, lo ejecuta como comando externo
 *
 * Retorna:
 *   El código de salida del comando ejecutado
 */
int	execute_cmd(t_shell *shell)
{
	int	ret;
	int	saved_stdin;
	int	saved_stdout;

	if (!shell || !shell->cmd || !shell->cmd->args || !shell->cmd->args[0])
		return (1);

	if (!shell->cmd->next && is_builtin(shell->cmd->args[0]))
	{
		saved_stdin = dup(0);
		saved_stdout = dup(1);

		if (shell->cmd->redirs && apply_redirections(shell->cmd, shell))
			return (1);

		ret = execute_builtin(shell->cmd, shell);

		dup2(saved_stdin, 0);
		dup2(saved_stdout, 1);
		close(saved_stdin);
		close(saved_stdout);

		return (ret);
	}

	if (shell->cmd->next)
		return (execute_piped_commands(shell->cmd, shell));

	ret = execute_external(shell->cmd, shell);
	if (ret == 127)
		handle_command_not_found(shell->cmd->args[0], shell);
	return (ret);
}
