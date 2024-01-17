#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 12345;

int main() 
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
	{
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
	{
        std::cerr << "Error connecting to server" << std::endl;
        close(sockfd);
        return 1;
    }

    std::string message = "journee de mai";
    send(sockfd, message.c_str(), message.size(), 0);

    close(sockfd);

    return 0;
}