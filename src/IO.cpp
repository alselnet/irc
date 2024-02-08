/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IO.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:45:31 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 15:24:40 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

int add_client(int fd, int epollFd)
{
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = fd;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		std::cerr << "Error adding server socket to epoll" << std::endl;
		close(epollFd);
		return (-1);
	}
	return (0);
}

int	receive_transmission(int clientSockFd, irc * irc_data)
{
	ssize_t	bytes;
	char	buffer[BUFFER_SIZE];

	memset(buffer, 0, BUFFER_SIZE);

	bytes = recv(clientSockFd, buffer, BUFFER_SIZE - 1, 0);
	if (bytes < 0)
		std::cerr << "Error receiving data" << std::endl;
	else if (!bytes) // WIP => Check si condition necessaire
	{
		std::cout << "Client disconnected" << std::endl;
		delete_user(clientSockFd, irc_data);
		std::cout << "Users List is now : " << std::endl;
		printContainer(irc_data->usersList);
		std::cout << "\n";
	}
	else
	{
		parse_transmission(buffer, clientSockFd, irc_data);
		memset(buffer, 0, BUFFER_SIZE);
	}
	return (bytes);
}
