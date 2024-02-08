/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:11:58 by aselnet           #+#    #+#             */
//   Updated: 2024/02/08 21:48:57 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

# include <sys/socket.h>
# include <fcntl.h>
# include <sys/epoll.h>
# include <iostream>
# include <cstring>
# include <csignal>
# include <sstream>
# include <fstream>
# include <vector>
# include <map>
# include <list>
# include <algorithm>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <string>

# include "color.h"
# include "User.hpp"
# include "Channel.hpp"

const int PORT = 6667;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 12; // 2 more for server socket and stdin

struct irc
{
	std::list<User>		usersList;
	std::list<Channel>	channelList;
};

//Server id strings
const std::string SERVER_NAME = "The_new_whatsapp";
const std::string SERVER_BIRTH = "Jan 1st 2024";
const std::string SERVER_VERS = "0.0.0.2";
const std::string SERVER_UMODES = "i";
const std::string SERVER_CMODES = "itkol";

//server init
int		setup_signal(void);
int		bind_socket(int serverSockFd);
int		server_setup();
void	set_non_blocking(int &fd);

//welcome page
void	handshake_replies(int clientSockFd, std::string target_nickame);

//input output management
int 	add_client(int fd, int epollFd);
int		receive_transmission(int clientSockFd, irc * irc_data);

//commands
void	pong(int target_fd);

//server loop
void 		handle_signal(int signal);
int		handle_new_connection(int serverSockFd);
int		server_loop(void);

//parsing
void	parse_transmission( char * buffer, int clientSockFd, irc * irc_data);

//getters
std::list<User>::iterator get_user( int clientSockFd, irc * irc_data );
std::list<Channel>::iterator	get_channel(std::string chan_name, irc *irc_data);
std::list<User>::iterator get_user_by_nick( std::string nickname, irc * irc_data );

//private_msg
void	private_msg( std::string str, irc * irc_data, int clientSockFd);

//quit
void close_all(irc *irc_data, int epollFd, int serverSockFd);
void	delete_user( int sockFd, irc * irc_data );\

//debug
template <typename T>
void	printContainer( T container );

#endif
