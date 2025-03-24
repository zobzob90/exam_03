/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:50:02 by eric              #+#    #+#             */
/*   Updated: 2025/03/24 16:03:56 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 30
#endif

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		*s++;
	}
	return (NULL);
}

size_t	ft_strlen(const char *str)
{
	size_t i = 0;

	while (str[i])
		i++;
	return (i);
}

void	ft_strcpy(char *s1, const char *s2)
{
	while (*s2)
		*s1++ = *s2++;
	*s1 = '\0';
}

char	*ft_strdup(const char *src)
{
	size_t len = ft_strlen(src) + 1;
	char *dest = malloc(sizeof(char *) * (len));
	if (!dest)
		return (NULL);
	ft_strcpy(dest, src);
	return (dest);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (NULL);
	size_t s1_len = ft_strlen(s1);
	size_t s2_len = ft_strlen(s2);
	char	*join = malloc(sizeof(char *) * (s1_len + s2_len) + 1);
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy((join + s1_len), s2);
	free(s1);
	return (join);
}

char	*get_next_line(int fd)
{
	static char buf[BUFFER_SIZE + 1];
	char	*line;
	char	*newline;
	int		countread;
	int		to_copy;

	line = ft_strdup(buf);
	while (!(newline = ft_strchr(line, '\n')) && (countread = read(fd, buf, BUFFER_SIZE)))
	{
		buf[countread] = '\0';
		line = ft_strjoin(line, buf);	
	}
	if (ft_strlen(line) == 0)
		return (free(line), NULL);
	if (newline != NULL)
	{
		to_copy = newline - line + 1;
		ft_strcpy(buf, newline + 1);
	}
	else
	{
		to_copy = ft_strlen(line);
		buf[0] = '\0';
	}
	line[to_copy]= '\0';
	return (line);
}

int main(void)
{
	int fd = open("test", O_RDONLY);
	char *line;
	int nb = 1;

	while ((line = get_next_line(fd)))
	{
		printf("Ligne %d : %s", nb, line);
		nb++;
		free(line);
	}
	close(fd);
	return (0);
}
