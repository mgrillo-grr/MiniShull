/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: is_builtin
 * ------------------
 * Verifica si un comando es un builtin del shell.
 *
 * Parámetros:
 *   cmd: Nombre del comando a verificar
 *
 * Retorna:
 *   1 si el comando es un builtin
 *   0 si no lo es
 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strncmp(cmd, "echo", 5) ||
			!ft_strncmp(cmd, "cd", 3) ||
			!ft_strncmp(cmd, "pwd", 4) ||
			!ft_strncmp(cmd, "export", 7) ||
			!ft_strncmp(cmd, "unset", 6) ||
			!ft_strncmp(cmd, "env", 4) ||
			!ft_strncmp(cmd, "exit", 5) ||
			!ft_strncmp(cmd, "history", 8));
}

/*
 * Función: execute_builtin
 * ----------------------
 * Ejecuta un comando builtin.
 *
 * Parámetros:
 *   cmd: Estructura del comando a ejecutar
 *   shell: Estructura principal del shell
 *
 * Retorna:
 *   El código de salida del comando ejecutado
 *   -1 si el comando no es un builtin
 */
int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	char	*command;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);
	command = cmd->args[0];
	if (!ft_strncmp(command, "echo", 5))
		return (cmd_echo(shell, cmd->args));
	if (!ft_strncmp(command, "cd", 3))
		return (cmd_cd(shell, cmd->args));
	if (!ft_strncmp(command, "pwd", 4))
		return (cmd_pwd(shell, cmd->args));
	if (!ft_strncmp(command, "export", 7))
		return (cmd_export(shell, cmd->args));
	if (!ft_strncmp(command, "unset", 6))
		return (cmd_unset(shell, cmd->args));
	if (!ft_strncmp(command, "env", 4))
		return (cmd_env(shell, cmd->args));
	if (!ft_strncmp(command, "exit", 5))
		return (cmd_exit(shell, cmd->args));
	if (!ft_strncmp(command, "history", 8))
		return (cmd_history(shell, cmd->args));
	return (-1);
}
