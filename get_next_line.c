/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpolonen <tpolonen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 17:22:03 by tpolonen          #+#    #+#             */
/*   Updated: 2021/12/10 15:43:23 by tpolonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	update_buff(t_buff *buff)
{
	buff->read = 0;
	buff->bytes = read(buff->fd, buff->content, BUFF_SIZE);
	if (buff->bytes > 0)
		buff->content[buff->bytes] = '\0';
	else
		buff->content[0] = '\0';
}

static t_buff	*new_buff(const int fd)
{
	t_buff	*new_buff;	

	new_buff = (t_buff *) ft_memalloc(sizeof(t_buff));
	if (new_buff)
	{
		new_buff->content = (char *) malloc(sizeof(char) * (BUFF_SIZE + 1));
		new_buff->fd = fd;
	}
	return (new_buff);
}

static t_buff	*get_buff(const int fd, t_buff **bufs)
{
	t_buff	*seek;

	if (*bufs == NULL)
		*bufs = new_buff(fd);
	seek = *bufs;
	while (seek->fd != fd)
	{
		if (seek->next == NULL)
			seek->next = new_buff(fd);
		seek = seek->next;
	}
	return (seek);
}	

static int	read_fd(t_buff *buff, char **line)
{
	t_dstr	*new_line;
	char	*stop;

	new_line = ft_dstrnew("", 512);
	while (buff->bytes > 0)
	{
		stop = ft_strchrnul(buff->content + buff->read, '\n');
		ft_dstradd(new_line, buff->content + buff->read,
			(stop - buff->read) - buff->content);
		buff->read = stop - buff->content + 1;
		if (buff->read <= buff->bytes)
			break ;
		update_buff(buff);
	}
	if (!line)
		ft_dstrfree(new_line);
	else
		*line = ft_dstrbreak(new_line);
	return (1);
}

int	get_next_line(const int fd, char **line)
{
	static t_buff	*bufs;
	t_buff			*fd_buff;

	fd_buff = get_buff(fd, &bufs);
	if (fd_buff->bytes <= 0 || fd_buff->read >= fd_buff->bytes)
	{
		update_buff(fd_buff);
		if (fd_buff->bytes <= 0)
			return (fd_buff->bytes);
	}
	return (read_fd(fd_buff, line));
}
