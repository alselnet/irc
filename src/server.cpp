# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

volatile sig_atomic_t exitFlag = 0;

int	bind_socket(int serverSockFd)
{
    struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;


    if (bind(serverSockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
	{
        std::cerr << "Error binding socket" << std::endl;
        close(serverSockFd);
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
		memset(buffer, 0, BUFFER_SIZE);
	}
	return (bytes);
}

void	set_non_blocking(int &fd)
{
	int	flags;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags < 0)
	{
		std::cerr << "Error setting up socket flags" << std::endl;
		return ;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		std::cerr << "Error setting up non_blocking on socket" << std::endl;
		return ;
	}
	return ;
}

void close_all(int *clientFds, int epollFd, int serverSockFd, int clientNb) 
{
    for (int i = 0; i < clientNb; ++i) 
	{
		close(clientFds[i]);
    }
	close(epollFd);
	close(serverSockFd);
	return ;
}

int	server_setup()
{
	int serverSockFd;

	serverSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSockFd == -1)
	{
        std::cerr << "Error creating socket" << std::endl;
        return (-1);
    }

	const int enable = 1;
	if (setsockopt(serverSockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		 std::cerr << "Error setting reuse mode" << std::endl;
        return (-1);
	}

	if (bind_socket(serverSockFd) == -1)
		return (-1);
	return (serverSockFd);
}

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
void handle_signal(int signal) 
{
    if (signal == SIGINT) 
	{
        std::cout << "\nReceived SIGINT. Cleaning up and exiting..." << std::endl;
        exitFlag = 1;
    }
}

int	handle_new_connection(int serverSockFd)
{
	int					clientSockFd;
	int					flags;
    struct sockaddr_in	clientAddr;
	std::string			RPL_WELCOME;
	std::string			RPL_YOURHOST;
	std::string			RPL_CREATED;
	std::string			RPL_MYINFO;

	RPL_WELCOME = ":the_new_whatsapp 001 abc :Welcome to the new WhatsApp abc\r\n";
	RPL_YOURHOST = ":the_new_whatsapp 002 abc :Your host is the_new_whatsapp, running version 0.0.0.1\r\n";
	RPL_CREATED = ":the_new_whatsapp 003 abc :This server was created on 01/01/24\n";
	RPL_MYINFO = ":the_new_whatsapp 004 abc :the_new_whatsapp 0.0.0.1 beim itkol\r\n";

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
		send(clientSockFd, RPL_WELCOME.c_str(), RPL_WELCOME.size(), 0);
		send(clientSockFd, RPL_YOURHOST.c_str(), RPL_YOURHOST.size(), 0);
		send(clientSockFd, RPL_CREATED.c_str(), RPL_CREATED.size(), 0);
		send(clientSockFd, RPL_MYINFO.c_str(), RPL_MYINFO.size(), 0);
	}
	return(clientSockFd);
}

int	setup_signal()
{
	struct sigaction	sa;

	std::memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		std::cerr << "Error setting up signal handler" << std::endl;
		return (-1);
	}
	return (0);
}

int	server_loop()
{
	int serverSockFd;
	int	clientSockFd;
	int clientNb;
	int	epollFd;
	int	eventsNb;

	int					clientFds[MAX_CLIENTS];
	std::list<User>		usersList;

	clientNb = 0;

	if (setup_signal() < 0)
		return (-1);

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
	while (!exitFlag)
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
				std::getline(std::cin, input);
				if (input.compare("exit") == 0)
					close_all(clientFds, epollFd, serverSockFd, clientNb);
				else
				{
					for (int i = 0; i < clientNb; ++i)
						send(clientFds[i], input.c_str(), input.size(), 0);
				}
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
