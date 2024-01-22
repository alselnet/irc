#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 12345;

int main( void ) 
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
    serverAddr.sin_addr.s_addr = inet_addr("78.203.158.77");

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
	{
        std::cerr << "Error connecting to server lol" << std::endl;
        close(sockfd);
        return 1;
    }

    // char input[1000];
    std::string string;

    while (1)
    {
        std::cin >> string;
        // string = input;
        std::cout << string << std::endl;
        send(sockfd, string.c_str(), string.size(), 0);
        std::cout << string << std::endl;
        string.clear();
    }

    // std::string message = "journee de mai";
    // std::string message2 = "journee d'avril";
    // send(sockfd, message.c_str(), message.size(), 0);
	// sleep(0.01);
    // send(sockfd, message2.c_str(), message2.size(), 0);

    close(sockfd);

    return 0;
}