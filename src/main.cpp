/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:10:24 by aselnet           #+#    #+#             */
/*   Updated: 2024/01/31 16:51:42 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

int	server_loop()
{
	int serverSockFd;
	int	clientSockFd;
	int clientNb;
	int	epollFd;
	int	eventsNb;

	int clientFds[MAX_CLIENTS];
	std::list<User>	usersList;

	clientNb = 0;

    serverSockFd = server_setup();
	if (serverSockFd < 0)
		return (-1);
	epollFd = epoll_create(MAX_CLIENTS);
	//epollFd = epoll_create1(0); //no client limit
	if (epollFd == -1)
	{
		std::cerr << "Error initializing epoll" << std::endl;
		close(serverSockFd);
		return (-1);
	}

	// Adds the server socket to epoll events
	if (add_client(serverSockFd, epollFd) < 0)
		return (-1);
	//adding stdin to epoll events for shutting down the server
	if(add_client(0, epollFd) < 0)
		return(-1);

	//starting epoll control loop;
	epoll_event events[MAX_CLIENTS];
	while (true)
	{
		eventsNb  = epoll_wait(epollFd, events, MAX_CLIENTS, -1); // -1 for no timeouts;
		if (eventsNb == -1)
			break ;
		for (int i = 0; i < eventsNb; i++)
		{
			if (events[i].data.fd == serverSockFd) // incoming connection request on server socket
			{
				clientSockFd = handle_new_connection(serverSockFd);
				if (clientSockFd == -1)
					break ;
				else
				{
					if (add_client(clientSockFd, epollFd) < 0)
					{
						std::cerr << "Error adding new client socket to epoll" << std::endl;
						close(clientSockFd);
					}
					else
					{
						clientFds[clientNb] = clientSockFd;
						clientNb++;
					}
				}
			}
			else if (events[i].data.fd == 0) // receiving from stdin
			{
				std::string input;
				input.clear();
				std::cin >> input;
				if (input.compare("exit") == 0)
					close_all(clientFds, epollFd, serverSockFd, clientNb);
			}
			else // receiving transmission from already connected client
			{
				int bytes = receive_transmission(events[i].data.fd, usersList);
				if (bytes < 1)
				{
					epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
					close(events[i].data.fd);
					clientNb--;
				}
			}
		}
	}
	close_all(clientFds, epollFd, serverSockFd, clientNb);
    return (0);
}

int main( int argc, char **argv)
{
	(void) argc, (void) argv;

	// Channel	foo = Channel("Test");
	// User		ufoo = User("0.0.0.0");
	
	// ufoo.setNickname("Jules");
	// foo.addUser(ufoo);

	// User		vfoo = User("1.1.1.1");
	
	// vfoo.setNickname("Alex");
	// foo.addUser(vfoo);

	// foo.kickUser(vfoo);
	// foo.kickUser(vfoo);

	if (server_loop() < 0)
		return (-1);
	return (0);
}