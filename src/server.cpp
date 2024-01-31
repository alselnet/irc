# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

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

int	handle_new_connection(int serverSockFd)
{
	int					clientSockFd;
    struct sockaddr_in	clientAddr;

	if (listen(serverSockFd, 1) == -1)
	{
		std::cerr << "Error listening on server socket" << std::endl;
		close(serverSockFd);
		return (-1);
	}

	socklen_t clientAddrLen = sizeof(clientAddr);
	clientSockFd = accept(serverSockFd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
	if (clientSockFd == -1)
	{
		std::cerr << "Incoming client connection refused" << std::endl;
		return (-1);
	}
	std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
	return(clientSockFd);
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
		parse_transmission(buffer, usersList);
		// std::cout << "Received message: " << buffer << std::endl;
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

	//set_non_blocking(fd);
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		std::cerr << "Error adding server socket to epoll" << std::endl;
		close(epollFd);
		return (-1);
	}
	return (0);
}
