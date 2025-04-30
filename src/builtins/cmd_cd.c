/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: update_pwd
 * ------------------
 * Actualiza las variables de entorno PWD y OLDPWD después de un cambio de directorio.
 *
 * Parámetros:
 *   env: Puntero a la lista de variables de entorno
 *
 * Función: cmd_cd
 * -------------
 * Implementa el comando builtin 'cd' para cambiar el directorio de trabajo.
 *
 * Parámetros:
 *   args: Array de strings con los argumentos del comando
 *         args[0] es "cd"
 *         args[1] es la ruta destino (opcional)
 *   env: Puntero a la lista de variables de entorno
 *
 * Comportamiento:
 * 1. Si no se proporciona ruta (args[1] es NULL):
 *    - Intenta cambiar al directorio HOME
 *    - Si HOME no está definido, muestra error
 *
 * 2. Si se proporciona ruta:
 *    - Intenta cambiar al directorio especificado
 *    - La ruta puede ser absoluta o relativa
 *
 * 3. Después de un cambio exitoso:
 *    - Actualiza PWD y OLDPWD
 *
 * Retorna:
 *   0: Éxito - el directorio se cambió correctamente
 *   1: Error - no se pudo cambiar el directorio o HOME no está definido
 *
 * Ejemplos de uso:
 *   cd          -> cambia al directorio HOME
 *   cd /path    -> cambia al directorio especificado
 *   cd ..       -> sube un nivel en el árbol de directorios
 */
int	cmd_cd(t_shell *shell, char **args)
{
	char	*path;

	/* Si no hay argumentos, usa HOME como destino */
	if (!args[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
		{
			ft_putendl_fd(ERR_PREFIX"cd"ERR_HOME, 2);
			return (1);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", 2);
			return (1);
		}
		ft_putendl_fd(path, 1);
	}
	else
		path = ft_strdup(args[1]);

	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free(path);
		return (1);
	}

	char old_pwd[PATH_MAX];
	char *old_pwd_str = get_env_value(shell->env, "PWD");
	if (old_pwd_str)
		set_env_value(shell->env, "OLDPWD", old_pwd_str);

	if (getcwd(old_pwd, sizeof(old_pwd)))
		set_env_value(shell->env, "PWD", old_pwd);

	free(path);
	return (0);
}
