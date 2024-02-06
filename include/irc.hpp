/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:11:58 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 03:33:34 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

# include <sys/socket.h>
// # include <netdb.h>
# include <fcntl.h>
# include <sys/epoll.h>
# include <iostream>
# include <cstring>
// # include <cerrno>
# include <csignal>
# include <sstream>
# include <fstream>
# include <map>
# include <list>
# include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

# include "color.h"
# include "User.hpp"
# include "Channel.hpp"

const int PORT = 6667;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 12; // 2 more for server socket and stdin

//Server id strings
const std::string SERVER_NAME = "The_new_whatsapp";
const std::string SERVER_BIRTH = "Jan 1st 2024";
const std::string SERVER_VERS = "0.0.0.2";
const std::string SERVER_UMODES = "beim";
const std::string SERVER_CMODES = "itkol";

int	bind_socket(int serverSockFd);
int	handle_new_connection(int serverSockFd);
int	receive_transmission(int clientSockFd, std::list< User > usersList);
void	set_non_blocking(int &fd);
void close_all(int *clientFds, int epollFd, int serverSockFd, int clientNb);
int	server_setup();
int add_client(int fd, int epollFd);
void	parse_transmission( char * buffer, std::list< User > usersList);
void handle_signal(int signal);
int	server_loop(void);


#endif