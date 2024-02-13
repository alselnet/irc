/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:23:05 by aselnet           #+#    #+#             */
//   Updated: 2024/02/13 11:17:05 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/channel_command.hpp"

typedef void	(*command_ptr)(std::string arg, int clientSockFd, irc *irc_data);

void	execute_command(std::string str, int clientSockFd, irc *irc_data)
{
	std::string			arg;
	std::string			args;
	std::string			cmd;
	std::istringstream	iss(str);
	std::string			commands[10];
	command_ptr			fcts[10];
	int 				i;

	i = 0;
	iss >> cmd;

	commands[0] = "NICK";
	commands[1] = "USER";
	commands[2] = "PRIVMSG";
	commands[3] = "PING";
	commands[4] = "TOPIC";
	commands[5] = "KICK";
	commands[6] = "INVITE";
	commands[7] = "MODE";
	commands[8] = "JOIN";
	commands[9] = "PART";

	fcts[0] = &nick;
	fcts[1] = &user;
	fcts[2] = &private_msg;
	fcts[3] = &pong;
	fcts[4] = &topic_change;
	fcts[5] = &kick_user;
	fcts[6] = &invite_user;
	fcts[7] = &mode_change;
	fcts[8] = &channel_join;
	fcts[9] = &channel_leave;

	while (i < 10 && commands[i].compare(cmd) != 0)
		i++;
	if (i < 2)
	{
		iss >> arg;
		fcts[i](arg, clientSockFd, irc_data);
	}
	else if (i == 3)
	{
		std::getline(iss, args);
		fcts[i](args, clientSockFd, irc_data);
	}
	else if (i > 9)
		std::cerr << "command not found" << std::endl;
	else
		fcts[i](str, clientSockFd, irc_data);
	return ;
}
