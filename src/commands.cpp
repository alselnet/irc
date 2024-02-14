/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:23:05 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/14 16:53:00 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/channel_command.hpp"

typedef void	(*command_ptr)(std::string arg, int clientSockFd, irc *irc_data);

void	execute_command(std::string str, int clientSockFd, irc *irc_data)
{
//	std::cerr << "DEBUG: execute_command starting" << std::endl;
	std::string			arg;
	std::string			args;
	std::string			cmd;
	std::istringstream	iss(str);
	std::string			commands[11];
	command_ptr			fcts[11];
	int 				i;

	i = 0;
	iss >> cmd;

	commands[0] = "NICK";
	commands[1] = "userhost";
	commands[2] = "USER";
	commands[3] = "PRIVMSG";
	commands[4] = "PING";
	commands[5] = "TOPIC";
	commands[6] = "KICK";
	commands[7] = "INVITE";
	commands[8] = "MODE";
	commands[9] = "JOIN";
	commands[10] = "PART";

	fcts[0] = &nick;
	fcts[1] = &userhost;
	fcts[2] = &set_user_infos;
	fcts[3] = &private_msg;
	fcts[4] = &pong;
	fcts[5] = &topic_change;
	fcts[6] = &kick_user;
	fcts[7] = &invite_user;
	fcts[8] = &mode_change;
	fcts[9] = &channel_join;
	fcts[10] = &channel_leave;

	while (i < 11 && commands[i].compare(cmd) != 0)
		i++;
	if (i < 2)
	{
		iss >> arg;
		fcts[i](arg, clientSockFd, irc_data);
	}
	else if (i == 2)
	{
		std::getline(iss, args);
		fcts[i](args, clientSockFd, irc_data);
	}
	else if (i > 10)
		std::cerr << "command not found" << std::endl;
	else
		fcts[i](str, clientSockFd, irc_data);
//	std::cerr << "DEBUG: execute_command ended successfully" << std::endl;
	return ;
}
