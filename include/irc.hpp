/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:11:58 by aselnet           #+#    #+#             */
/*   Updated: 2024/03/05 16:20:20 by jthuysba         ###   ########.fr       */
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

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 12; // 2 more for server socket and stdin

struct irc
{
	std::list<User>		usersList;
	std::list<Channel>	channelList;
	std::string			password;
	int					port;
};

//Server id strings
const std::string SERVER_NAME = "The_new_whatsapp";
const std::string SERVER_BIRTH = "Jan 1st 2024";
const std::string SERVER_VERS = "0.0.0.5";
const std::string SERVER_UMODES = "oO";
const std::string SERVER_CMODES = "itkol";

//server init
int		setup_signal(void);
int		bind_socket(int serverSockFd, irc *irc_data);
int		server_setup(irc *irc_data);
void	set_non_blocking(int &fd);

//welcome page
void	handshake_replies(int clientSockFd, std::string target_nickame);

//input output management
int 	add_client(int fd, int epollFd);
int		receive_transmission(int clientSockFd, irc * irc_data);

//commands
void	execute_command(std::string str, int *clientSockFd, irc *irc_data);
void	pong(std::string *args, int *target_fd, irc *irc_data);
void	nick(std::string *arg, int *clientSockFd, irc *irc_data);
void 	usercmd(std::string *arg, int *clientSockFd, irc *irc_data);
void	set_user_infos(std::string *str, int *clientSockFd, irc *irc_data);
void	password_check(std::string *arg, int *clientSockFd, irc *irc_data);
void 	dcc(std::string *arg, int *clientSockFd, irc *irc_data);

//server loop
void 	handle_signal(int signal);
int		handle_new_connection(int serverSockFd);
int		server_loop(irc *irc_data);

//parsing
void	parse_transmission( char * buffer, int *clientSockFd, irc * irc_data);
std::list<User>::iterator	findUser(std::string nickname, std::list<User> &usersList);

//getters
std::list<User>::iterator 		get_user( int clientSockFd, irc * irc_data );
std::list<Channel>::iterator	get_channel(std::string chan_name, irc *irc_data);
std::list<User>::iterator 		get_user_by_nick( std::string nickname, irc * irc_data );

//private_msg
void	private_msg( std::string *str, int *clientSockFd, irc * irc_data);

//quit
void 	close_all(irc *irc_data, int epollFd, int serverSockFd);
void	delete_user( int sockFd, irc * irc_data );\

//debug
void	printUsersList( std::list<User> & list );
void	print_user_infos(std::list<User>::iterator &user);

#endif
