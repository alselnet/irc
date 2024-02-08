/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/08 21:58:18 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Notif.hpp"
# include "../include/channel_command.hpp"

// Execute la commande dans str
void	execute_command( std::string str, int clientSockFd, irc * irc_data )
{
	std::string			cmd;
	std::istringstream	iss(str);

	iss >> cmd;
	//std::cerr << "str:cmd = [" << str << "] [" << cmd << "]" << std::endl;
	// if (cmd == "CAP")
	// {
		// WIP => Gerer capacite ?
	// }
	if (cmd == "NICK")
	{
		// WIP => Check si nick deja utilise
		std::string	nick;
		
		iss >> nick;
		if (get_user(clientSockFd, irc_data)->getNickname().empty())
		{
			get_user(clientSockFd, irc_data)->setNickname(nick);
			handshake_replies(clientSockFd, get_user(clientSockFd, irc_data)->getNickname());
		}
		else
			get_user(clientSockFd, irc_data)->setNickname(nick);
	}
	else if (cmd == "PRIVMSG")
	{
		std::string	args;
		std::getline(iss, args);

		private_msg(args, irc_data, clientSockFd);
	}
	else if (cmd == "PING")
	{
		pong(clientSockFd);
	}
	else if (cmd == "TOPIC")
	{
		topic_change(str, clientSockFd, irc_data);
	}
	else if (cmd == "KICK")
	{
		kick_user(str, clientSockFd, irc_data);
	}
	else if (cmd == "INVITE")
	{
		invite_user(str, clientSockFd, irc_data);
	}
	else if (cmd == "MODE")
	{
		mode_change(str, clientSockFd, irc_data);
	}
	else if (cmd == "JOIN")
	{
		channel_join(str, clientSockFd, irc_data);
	}
//	else if (cmd == "USER")
//	{
		// WIP => Coder une fonction qui gere "USER <username> <username> <ip> :<realname>" & "userhost <username>" (a voir)
//	}
	// WIP => Toutes les autres commandes a ajoute
}


// parse la transmission ligne par ligne et execute chaque commande
void	parse_transmission( char * buffer, int clientSockFd, irc * irc_data )
{
	(void) irc_data;
	(void) clientSockFd;
	
	const std::string		str(buffer);
	std::istringstream	iss(str);
	std::string				line;
	
	while (std::getline(iss, line, '\r'))
	{
		if (!line.empty())
		{
			std::cout << "[" << YELLOW << line << RESET << "]" << std::endl;
			std::cout << "Sent by : " << CYAN << get_user(clientSockFd, irc_data)->getNickname() << END << std::endl;
			execute_command(line, clientSockFd, irc_data);
		}
		iss.ignore();
	}
}
