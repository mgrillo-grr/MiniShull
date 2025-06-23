/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrillo <mgrillo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:56 by mgrillo           #+#    #+#             */
/*   Updated: 2025/04/30 15:29:56 by mgrillo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

/*
 * Función: create_cmd
 * ------------------
 * Crea una nueva estructura de comando.
 *
 * Retorna:
 *   Nueva estructura de comando inicializada
 *   NULL si hay un error de memoria
 */
static t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

/*
 * Función: free_cmd
 * ----------------
 * Libera la memoria de una estructura de comando.
 *
 * Parámetros:
 *   cmd: Estructura de comando a liberar
 */
void	free_cmd(t_cmd *cmd)
{
	t_cmd	*next;
	t_redir	*redir;
	t_redir	*next_redir;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
			free_array(cmd->args);
		redir = cmd->redirs;
		while (redir)
		{
			next_redir = redir->next;
			free(redir->file);
			free(redir);
			redir = next_redir;
		}
		free(cmd);
		cmd = next;
	}
}

/*
 * Función: parse_input
 * -------------------
 * Parsea una línea de entrada y crea una estructura de comando.
 *
 * Parámetros:
 *   input: Línea de entrada a parsear
 *   shell: Estructura principal del shell
 *
 * Proceso:
 * 1. Crea una nueva estructura de comando
 * 2. Divide la entrada en argumentos usando espacios como delimitador
 * 3. Almacena los argumentos en la estructura
 *
 * Retorna:
 *   Estructura de comando con los argumentos
 *   NULL si hay un error
 */
/*
 * Función: add_redirection
 * -----------------------
 * Añade una redirección a la lista de redirecciones del comando.
 */
static int	add_redirection(t_cmd *cmd, char **tokens, int *i)
{
	t_redir	*redir;
	t_redir	*last;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (0);

	if (ft_strcmp(tokens[*i], "<") == 0)
		redir->type = 1;
	else if (ft_strcmp(tokens[*i], ">") == 0)
		redir->type = 2;
	else if (ft_strcmp(tokens[*i], ">>") == 0)
		redir->type = 3;
	else if (ft_strcmp(tokens[*i], "<<") == 0)
		redir->type = 4;

	// Verificar que hay un archivo después del redirector
	if (!tokens[*i + 1])
	{
		free(redir);
		return (0);
	}

	redir->file = ft_strdup(tokens[*i + 1]);
	redir->next = NULL;

	// Asegurarse de que el comando tenga un array de argumentos vacío
	// para indicar que es solo una redirección sin comando
	if (!cmd->args)
	{
		cmd->args = malloc(sizeof(char *));
		if (!cmd->args)
		{
			free(redir);
			return (0);
		}
		cmd->args[0] = NULL;
	}

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = redir;
	}

	return (1);
}

/*
 * Función: add_command
 * -------------------
 * Añade un comando a la lista de comandos.
 */
static t_cmd	*add_command(t_cmd *cmd_list)
{
	t_cmd	*new_cmd;
	t_cmd	*last;

	new_cmd = create_cmd();
	if (!new_cmd)
		return (NULL);

	if (!cmd_list)
		return (new_cmd);

	last = cmd_list;
	while (last->next)
		last = last->next;
	last->next = new_cmd;

	return (cmd_list);
}

/*
 * Función: create_empty_cmd
 * -----------------------
 * Crea un comando vacío con redirecciones.
 * 
 * Retorna:
 *   Nueva estructura de comando vacío inicializada
 */
// t_cmd *create_empty_cmd(void)
// {
// 	t_cmd *cmd = create_cmd();
// 	if (!cmd)
// 		return (NULL);
// 	cmd->args = malloc(sizeof(char *) * 2);
// 	if (!cmd->args)
// 	{
// 		free(cmd);
// 		return (NULL);
// 	}
// 	cmd->args[0] = ft_strdup("/bin/true");
// 	cmd->args[1] = NULL;
// 	return (cmd);
// }

/*
 * Función: parse_input
 * -------------------
 * Parsea una línea de entrada y crea una estructura de comando.
 */
t_cmd	*parse_input(char *input, t_shell *shell)
{
    t_cmd	*cmd_list;
    t_cmd	*current;
    char	**tokens;
    char	**args;
    int		i;
    int		arg_count;
    int		is_redirect; // Variable para rastrear si el token anterior era una redirección

    if (!input || !*input)
        return (NULL);

    tokens = tokenize(input);
    if (!tokens)
        return (NULL);

    cmd_list = NULL;
    current = NULL;
    args = NULL;
    arg_count = 0;
    is_redirect = 0; // Inicialmente no estamos procesando una redirección

    i = 0;
    while (tokens[i])
    {
        // Si encontramos una redirección, la procesamos inmediatamente
        if (tokens[i][0] == '<' || tokens[i][0] == '>')
        {
            // Marcar que estamos procesando una redirección
            is_redirect = 1;
            
            if (!current)
            {
                cmd_list = add_command(cmd_list);
                if (!cmd_list)
                {
                    free_array(tokens);
                    return (NULL);
                }
                current = cmd_list;
                while (current->next)
                    current = current->next;
                
                // Inicializar el comando con argumentos vacíos
                // para indicar que es solo una redirección sin comando
                if (!current->args)
                {
                    current->args = malloc(sizeof(char *));
                    if (!current->args)
                    {
                        free_array(tokens);
                        free_cmd(cmd_list);
                        return (NULL);
                    }
                    current->args[0] = NULL;
                }
            }
            
            // Procesar la redirección
            if (!add_redirection(current, tokens, &i))
            {
                free_array(tokens);
                free_cmd(cmd_list);
                return (NULL);
            }
            
            // Avanzar al siguiente token después de la redirección
            i++;
            continue;
        }
        else
        {
            // Si el token anterior era una redirección, este token es el nombre del archivo
            // No lo tratamos como un comando
            if (is_redirect)
            {
                is_redirect = 0; // Resetear el flag
                i++; // Avanzar al siguiente token
                continue;
            }
        }

        // Si encontramos un pipe, avanzamos
        if (ft_strcmp(tokens[i], "|") == 0)
        {
            if (!current->args)
            {
                free_array(tokens);
                free_cmd(cmd_list);
                return (NULL);
            }
            current = NULL;
            i++;
            continue;
        }

        // Si no es redirección ni pipe, es un argumento
        if (!current)
        {
            cmd_list = add_command(cmd_list);
            if (!cmd_list)
            {
                free_array(tokens);
                return (NULL);
            }
            current = cmd_list;
            while (current->next)
                current = current->next;
            args = NULL;
            arg_count = 0;
        }

        // Procesar el argumento
        char *expanded = NULL;
        int has_valid_var = 0;
        int k = 0;
        
        // Verificar si el token contiene variables que necesitan expansión
        while (tokens[i][k])
        {
            if (tokens[i][k] == '$' && tokens[i][k + 1] &&
                (ft_isalpha(tokens[i][k + 1]) || tokens[i][k + 1] == '_' || tokens[i][k + 1] == '?'))
            {
                has_valid_var = 1;
                break;
            }
            k++;
        }
        
        // Expandir el token si es necesario
        if (tokens[i][0] == '\'' || !has_valid_var)
        {
            expanded = ft_strdup(tokens[i]);
        }
        else
        {
            expanded = expand_token(tokens[i], shell);
        }

        if (!expanded)
        {
            if (args)
                free_array(args);
            free_array(tokens);
            free_cmd(cmd_list);
            return (NULL);
        }

        char *processed = expanded;
        
        // Asignar memoria para los nuevos argumentos
        char **new_args = malloc(sizeof(char *) * (arg_count + 2));
        if (!new_args)
        {
            // Free both processed and expanded if they're different
            free(processed);
            if (expanded != processed)
                free(expanded);
            if (args)
                free_array(args);
            free_array(tokens);
            free_cmd(cmd_list);
            return (NULL);
        }

        int j;
        for (j = 0; j < arg_count; j++)
            new_args[j] = args[j];

        new_args[arg_count] = processed;
        new_args[arg_count + 1] = NULL;
        arg_count++;

        if (args)
            free(args);
        args = new_args;
        current->args = args;
        i++;
    }

    free_array(tokens);
    return (cmd_list);
}