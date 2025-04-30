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

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(paths);
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

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);

	cmd_path = find_command_path(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		if (!cmd->redirs && !cmd->next)
			return (127);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}

	pid = fork();
	if (pid == 0)
	{
		if (cmd->redirs && apply_redirections(cmd, shell))
			exit(1);
		envp = env_to_array(shell->env);
		execve(cmd_path, cmd->args, envp);
		perror("minishell");
		exit(126);
	}
	free(cmd_path);
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

	return (execute_external(shell->cmd, shell));
}
