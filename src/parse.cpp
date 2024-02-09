/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/09 15:31:46 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Notif.hpp"
# include "../include/channel_command.hpp"

void	set_user_infos(std::string str, int clientSockFd, irc *irc_data)
{
	std::istringstream	iss(str);
	std::string				username;
	std::string				hostname;
	std::string				realname;
	std::string				dump;

	iss >> dump;
	iss >> username;
	iss >> dump;
	iss >> hostname;
	iss >> realname;

	realname.erase(0, 1);

	std::list<User>::iterator	user = get_user(clientSockFd, irc_data);

	user->setUsername(username);
	user->setIp(hostname);
	user->setRealname(realname);

	std::cout << user->getUsername() << std::endl;
	std::cout << user->getIp() << std::endl;
	std::cout << user->getRealname() << std::endl;

}

// Execute la commande dans str
void	execute_command(std::string str, int clientSockFd, irc *irc_data)
{
	std::string				cmd;
	std::istringstream	iss(str);

	iss >> cmd;

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
	else if (cmd == "USER")
	{
		set_user_infos(str, clientSockFd, irc_data);
	}
	// WIP => Toutes les autres commandes a ajoute
}


// parse la transmission ligne par ligne et execute chaque commande
void	parse_transmission(char *buffer, int clientSockFd, irc *irc_data)
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
