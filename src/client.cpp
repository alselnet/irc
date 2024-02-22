// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   client.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/22 09:59:49 by ctchen            #+#    #+#             //
//   Updated: 2024/02/22 09:59:53 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

const int PORT = 6667;

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
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
	{
        std::cerr << "Error connecting to server" << std::endl;
        close(sockfd);
        return 1;
    }
	std::string message;
	while(message.compare("exit") != 0)
	{
		message.clear();
		std::getline(std::cin, message);
		if (message.compare("exit") != 0)
			send(sockfd, message.c_str(), message.size(), 0);
		else
			send(sockfd, 0, 1, 0);
	}

    close(sockfd);

    return 0;
}
