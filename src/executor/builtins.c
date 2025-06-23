/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/27 15:42:34 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_utils.h"

/*
 * Función: is_builtin
 * ------------------
 * Verifica si un comando es un builtin.
 *
 * Parámetros:
 *   cmd: Nombre del comando a verificar
 *
 * Retorna:
 *   1 si el comando es un builtin
 *   0 si no lo es
 */
int	is_builtin(const char *cmd)
{
	const t_builtin	*builtins;
	size_t			i;

	if (!cmd)
		return (0);
	builtins = get_builtins();
	i = 0;
	while (i < get_builtins_count())
	{
		if (ft_strncmp(cmd, builtins[i].name,
				ft_strlen(builtins[i].name) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
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
	const t_builtin	*builtins;
	size_t			i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);
	builtins = get_builtins();
	i = 0;
	while (i < get_builtins_count())
	{
		if (ft_strncmp(cmd->args[0], builtins[i].name,
				ft_strlen(builtins[i].name) + 1) == 0)
			return (builtins[i].func(shell, cmd->args));
		i++;
	}
	return (-1);
}
