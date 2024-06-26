# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"
# include "../include/Reply.hpp"
# include "../include/Notif.hpp"

volatile sig_atomic_t exitFlag = 0;

void handle_signal(int signal) 
{
    if (signal == SIGINT) 
	{
        std::cout << "\nReceived SIGINT. Cleaning up and exiting..." << std::endl;
        exitFlag = 1;
    }
}

int	handle_new_connection(int serverSockFd, irc *irc_data)
{
	int					clientSockFd;
	int					flags;
	struct sockaddr_in	clientAddr;

	if (listen(serverSockFd, 1) == -1)
	{
		std::cerr << "Error listening on server socket" << std::endl;
		close(serverSockFd);
		return (-1);
	}

	socklen_t clientAddrLen = sizeof(clientAddr);
	clientSockFd = accept(serverSockFd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
	if (clientSockFd == -1 && !exitFlag)
	{
		std::cerr << "Incoming client connection refused" << std::endl;
		return (-1);
	}
	else if (!exitFlag)
	{
		std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
		flags = fcntl(clientSockFd, F_GETFL, 0);
		fcntl(clientSockFd, F_SETFL, flags | O_NONBLOCK);

		User	newUser(clientSockFd);
		newUser.setIp(inet_ntoa(clientAddr.sin_addr));
		irc_data->usersList.push_back(newUser);
	}
	return(clientSockFd);
}

void	timebot(unsigned int time_in_sec, irc *irc_data)
{
	time_t 		now = time(NULL);
	struct tm 	*tm_info = localtime(&now);
	char		time_str[80];
	strftime	(time_str, sizeof(time_str), "%H:%M", tm_info);

	static	time_t	last = 0;
	if (now - last >= time_in_sec)
	{
		for (std::list<Channel>::iterator it = irc_data->channelList.begin(); it != irc_data->channelList.end(); it++)
		{
			std::string time_message = "PRIVMSG " + std::string(it->getChName()) + " :*BEEB* *BOOP* local time is: " + time_str + "\r\n";
			for (std::list<std::string>::const_iterator it2 = it->getUsersListBegin(); it2 != it->getUsersListEnd(); it2++)
			{
				std::list<User>::const_iterator user_it = findUser((*it2), irc_data->usersList);
				if (user_it == irc_data->usersList.end())
					return;
				send (user_it->getSockFd(), time_message.c_str(), time_message.size(), 0);
				last = now;
			}
		}
	}

}

int	server_loop(irc *irc_data)
{
	int	serverSockFd;
	int	clientSockFd;
	int	epollFd;
	int	eventsNb;

	if (setup_signal() < 0)
		return (-1);

   serverSockFd = server_setup(irc_data);
	if (serverSockFd < 0)
		return (-1);
	epollFd = epoll_create(MAX_CLIENTS);
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
	while (!exitFlag)
	{
		eventsNb  = epoll_wait(epollFd, events, MAX_CLIENTS, -1); // -1 for no timeouts;
		if (eventsNb == -1)
			break ;
		for (int i = 0; i < eventsNb; i++)
		{
			if (events[i].data.fd == serverSockFd) // incoming connection request on server socket
			{
				clientSockFd = handle_new_connection(serverSockFd, irc_data);
				if (clientSockFd == -1)
					break ;
				else
				{
					if (add_client(clientSockFd, epollFd) < 0)
					{
						std::cerr << "Error adding new client socket to epoll" << std::endl;
						close(clientSockFd);
					}
					// else
					// {
					// 	User	newUser(clientSockFd);
					// 	irc_data.usersList.push_back(newUser);
					// }
				}
			}
			else if (events[i].data.fd == 0) // receiving from stdin
			{
				std::string input;
				std::getline(std::cin, input);
				if (input.compare("exit") == 0)
					close_all(irc_data, epollFd, serverSockFd);
				else
				{
					std::list<User>::iterator it;

					for (it = irc_data->usersList.begin(); it != irc_data->usersList.end(); ++it)
						send(it->getSockFd(), input.c_str(), input.size(), 0);
				}
			}
			else // receiving transmission from already connected client
			{
				int bytes = receive_transmission(events[i].data.fd, irc_data);
				if (bytes < 1)
					epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
			}
			timebot(300, irc_data);
		}
	}
	close_all(irc_data, epollFd, serverSockFd);
   return (0);
}
