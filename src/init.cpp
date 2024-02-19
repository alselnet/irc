/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:43:33 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/14 17:53:27 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

int	setup_signal(void)
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

void	set_non_blocking(int &fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		std::cerr << "Error setting up non_blocking on socket" << std::endl;

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