/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:22:39 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/27 17:22:39 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Function: is_executable
 * ----------------------
 * Checks if a file is executable.
 *
 * Parameters:
 *   path: Path to the file
 *
 * Returns:
 *   1 if the file exists and is executable
 *   0 otherwise
 */
static int	is_executable(const char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (1);
	return (0);
}

/*
 * Function: search_in_path
 * ----------------------
 * Searches for a command in the PATH directories.
 *
 * Parameters:
 *   cmd: Command name
 *   env: Environment variables
 *
 * Returns:
 *   Full path to the command if found
 *   NULL if not found
 */
static char	*search_in_path(const char *cmd, t_env *env)
{
	char	*path;
	char	**paths;
	char	*full_path;
	int		i;
	char	*temp;

	path = get_env_value(env, "PATH");
	if (!path || !*path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			i++;
			continue ;
		}
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (full_path && is_executable(full_path))
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	return (free_array(paths), NULL);
}

/*
 * Function: find_command_path
 * ----------------------
 * Finds the full path of a command in the PATH.
 *
 * Parameters:
 *   cmd: Command name
 *   env: Environment variables
 *
 * Returns:
 *   Full path to the command if found
 *   NULL if not found
 */
char	*find_command_path(char *cmd, t_env *env)
{
	t_quote_info	*cmd_info;
	char		*result;

	if (!cmd || !*cmd)
		return (NULL);
	cmd_info = remove_quotes(cmd);
	if (!cmd_info)
		return (NULL);
	if (cmd_info->str[0] == '/' || cmd_info->str[0] == '.')
	{
		if (is_executable(cmd_info->str))
		{
			result = ft_strdup(cmd_info->str);
			free(cmd_info->str);
			free(cmd_info);
			return (result);
		}
	}
	else
	{
		result = search_in_path(cmd_info->str, env);
		if (result)
		{
			free(cmd_info->str);
			free(cmd_info);
			return (result);
		}
	}
	free(cmd_info->str);
	free(cmd_info);
	return (NULL);
}
