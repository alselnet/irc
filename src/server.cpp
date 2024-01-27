#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

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
	int	clientSockFd;
    struct sockaddr_in clientAddr;
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

int	receive_transmission(int clientSockFd)
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
		std::cout << "Received message: " << buffer << std::endl;
		memset(buffer, 0, BUFFER_SIZE);
	}
	return (bytes);
}

int main() 
{
    int serverSockFd;
	int	clientSockFd;
	int	epollFd;

    serverSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSockFd == -1)
	{
        std::cerr << "Error creating socket" << std::endl;
        return (-1);
    }

	if (bind_socket(serverSockFd) == -1)
		return (-1);

	epollFd = epoll_create(MAX_CLIENTS + 2); // 2 more for server socket and stdin
	if (epollFd == -1)
	{
		std::cerr << "Error initializing epoll" << std::endl;
		close(serverSockFd);
		return (-1);
	}

	// Adds the server socket to epoll events
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = serverSockFd;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSockFd, &event) == -1)
	{
		std::cerr << "Error adding server socket to epoll" << std::endl;
		close(epollFd);
		close(serverSockFd);
		return (-1);
	}

	//adding stdin to epoll events for shutting down the server
	event.data.fd = 0;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, 0, &event) == -1)
	{
		std::cerr << "Error adding stdin event to epoll" << std::endl;
		close(epollFd);
		close(serverSockFd);
		return (-1);
	}

	//starting epoll control loop;
	epoll_event events[MAX_CLIENTS];
	while (true)
	{
		int eventsNb  = epoll_wait(epollFd, events, MAX_CLIENTS, -1); // -1 for no timeouts;
		if (eventsNb == -1)
		{
			std::cerr << "epoll_wait() error" << std::endl;
			break;
		}
		for (int i = 0; i < eventsNb; i++)
		{
			if (events[i].data.fd == serverSockFd) // incoming connection request on server socket
			{
				clientSockFd = handle_new_connection(serverSockFd);
				if (clientSockFd == -1)
					break ;
				else
				{
					event.events = EPOLLIN | EPOLLET;
					event.data.fd = clientSockFd;
					if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSockFd, &event) == -1)
					{
						std::cerr << "Error adding new client socket to epoll" << std::endl;
						close(serverSockFd);
					}
				}
			}
			else if (events[i].data.fd == 0) // receiving from stdin
			{
				// std::string input;
				// input.clear();
				// std::cin >> input;
				// if (input.compare("exit") == 0)
				// {
				// 	for (int i = 0; i < eventsNb; i++)
				// 		close(events[i].data.fd);
				// 	close (epollFd);
				// 	close (serverSockFd);
				// }
			}
			else // receiving transmission from already connected client
			{
				int bytes = receive_transmission(events[i].data.fd);
				if (bytes < 1)
				{
					epoll_ctl(epollFd, EPOLL_CTL_DEL, clientSockFd, NULL);
					close(clientSockFd);
				}
			}
		}
	}
	
    close(epollFd);
    close(serverSockFd);

    return (0);
}