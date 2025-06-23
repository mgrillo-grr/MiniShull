/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:15:00 by mgrillo           #+#    #+#             */
/*   Updated: 2025/05/28 14:15:00 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipes.h"

int	handle_external_command(t_cmd *cmd, t_shell *shell, char *cmd_path)
{
	char	**envp;

	envp = env_to_array(shell->env);
	if (!envp)
	{
		free(cmd_path);
		exit(1);
	}
	execve(cmd_path, cmd->args, envp);
	perror("minishell");
	free(cmd_path);
	free_array(envp);
	exit(126);
}

int	execute_external_command(t_cmd *cmd, t_shell *shell, t_quote_info *info)
{
	char	*cmd_path;

	cmd_path = find_command_path(info->str, shell->env);
	free(info->str);
	free(info);
	if (!cmd_path)
	{
		handle_command_not_found(cmd->args[0], shell);
		exit(127);
	}
	return (handle_external_command(cmd, shell, cmd_path));
}

int	execute_cmd_in_child(t_cmd *cmd, t_shell *shell)
{
	t_quote_info	*info;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(1);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, shell));
	info = remove_quotes(cmd->args[0]);
	if (!info)
		exit(127);
	execute_external_command(cmd, shell, info);
	return (126);
}
