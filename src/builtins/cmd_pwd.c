/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Función: cmd_pwd
 * -------------
 * Implementa el comando builtin 'pwd' (Print Working Directory).
 * Muestra el directorio de trabajo actual en la salida estándar.
 *
 * Parámetros:
 *   void: No recibe parámetros
 *
 * Comportamiento:
 * 1. Utiliza getcwd para obtener la ruta absoluta del directorio actual
 * 2. Si getcwd tiene éxito:
 *    - Imprime la ruta seguida de un salto de línea
 * 3. Si getcwd falla:
 *    - Muestra un mensaje de error
 *    - Esto puede ocurrir si:
 *      * No hay suficiente memoria
 *      * No hay permisos de lectura en algún directorio de la ruta
 *      * El directorio actual fue eliminado
 *
 * Retorna:
 *   0: Éxito - se mostró el directorio actual
 *   1: Error - no se pudo obtener el directorio actual
 *
 * Ejemplo de salida:
 *   pwd -> /home/usuario/directorio/actual
 */
int	cmd_pwd(t_shell *shell, char **args)
{
	char	cwd[PATH_MAX];

	(void)shell;
	(void)args;

	/* Intenta obtener el directorio actual */
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_putendl_fd(ERR_PREFIX"pwd: error retrieving current directory", 2);
		return (1);
	}
	/* Imprime el directorio actual seguido de un salto de línea */
	ft_putendl_fd(cwd, 1);
	return (0);
}
