/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IO.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:45:31 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 04:54:36 by aselnet          ###   ########.fr       */
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

int	receive_transmission(int clientSockFd, std::list< User > usersList)
{
	ssize_t	bytes;
	char	buffer[BUFFER_SIZE];

	memset(buffer, 0, BUFFER_SIZE);

	bytes = recv(clientSockFd, buffer, BUFFER_SIZE - 1, 0);
	if (bytes < 0)
		std::cerr << "Error receiving data" << std::endl;
	else if (!bytes)
		std::cout << "Client disconnected" << std::endl;
	else
	{
		(void) usersList;
		//parse_transmission(buffer, usersList);
		std::cout << "Received message: " << buffer << std::endl;
		
		//tmp code for testing PONG and MODE
		std::string	string;
		std::string cmd;
		string = buffer;
		size_t		pos = string.find(' ');
		cmd = string.substr(0, pos);
		if (!cmd.compare("PING"))
			pong(clientSockFd);
		else if (!cmd.compare("MODE"))
		{
			std::string mode_reply = ":" + SERVER_NAME + " MODE abc +i\r\n";
			send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
		}

		memset(buffer, 0, BUFFER_SIZE);
	}
	return (bytes);
}
