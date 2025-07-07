/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:12:25 by ertrigna          #+#    #+#             */
/*   Updated: 2025/07/03 15:28:41 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	err(char *str)
{
	int i = 0;
	while (str[i])
		write (2, &str[i++], 1);
}

int	cd(char **av, int i)
{
	if (i != 2)
		return (err("error: cd: bad arguments\n"), 1);
	else if (chdir(av[1]) != 0)
		return (err("error: cd: cannot change directory to "), err(av[1]), err("\n"), 1);
	return (0);
}

void	set_pipe(int has_pipe, int *fd, int end)
{
	if (has_pipe && (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
		err("error: fatal\n"), exit(1);
}

int	exec(char **av, int i, char **envp)
{
	int	has_pipe;
	int	fd[2];
	int status;
	int	pid;

	has_pipe = av[i] && !strcmp(av[i], "|");
	if (!has_pipe && !strcmp(*av, "cd"))
		return (cd(av, i));
	if (has_pipe && pipe(fd) == -1)
		err("error: fatal\n"), exit (1);
	pid = fork();
	if (pid == -1)
		err("error: fatal\n"), exit (1);
	if (!pid)
	{
		av[i] = 0;
		set_pipe(has_pipe, fd, 1);
		if (!strcmp(*av, "cd"))
			exit (cd(av, i));
		execve(*av, av, envp);
		err("error: cannot execute "), err(*av), err("\n"), exit (1);
	}
	waitpid(pid, &status, 0);
	set_pipe(has_pipe, fd, 0);
	return WIFEXITED(status) && WEXITSTATUS(status);
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	int	status = 0;
	int	i = 0;

	while (av[i])
	{
		av += i + 1;
		i = 0;
		while (av[i] && (strcmp(av[i], "|") && strcmp(av[i], ";")))
			i++;
		if (i)
			status = exec(av, i, envp);
	}
	return (status);
}
