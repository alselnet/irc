#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

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
	clientSockFd = accept(serverSockFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (clientSockFd == -1)
	{
		std::cerr << "Incoming client connection refused" << std::endl;
		return (-1);
	}
	return(clientSockFd);
}

void	receive_transmission(int clientSockFd)
{
    char buffer[BUFFER_SIZE];

	while (recv(clientSockFd, buffer, BUFFER_SIZE - 1, 0) > 0)
	{
		std::cout << "Received message: " << buffer << std::endl;
		memset(buffer, 0, BUFFER_SIZE);
	}
}

int main() 
{
    int serverSockFd;
	int	clientSockFd;

    serverSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSockFd == -1)
	{
        std::cerr << "Error creating socket" << std::endl;
        return (-1);
    }

	if (bind_socket(serverSockFd) == -1)
		return (-1);

	while (true)
	{
		clientSockFd = handle_new_connection(serverSockFd);
		if (clientSockFd == -1)
			return (-1);
		receive_transmission(clientSockFd);
	}
	
    close(clientSockFd);
    close(serverSockFd);

    return (0);
}
