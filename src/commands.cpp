/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:23:05 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/21 00:26:53 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/channel_command.hpp"

typedef void	(*command_ptr)(std::string *arg, int *clientSockFd, irc *irc_data);

void	execute_command(std::string str, int *clientSockFd, irc *irc_data)
{
//	std::cerr << "DEBUG: execute_command starting" << std::endl;
	std::string			arg;
	std::string			args;
	std::string			cmd;
	std::istringstream	iss(str);
	std::string			commands[12];
	command_ptr			fcts[12];
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
	commands[10] = "PASS";
	commands[11] = "DCC";

	fcts[0] = &nick;
	fcts[1] = &usercmd;
	fcts[2] = &private_msg;
	fcts[3] = &pong;
	fcts[4] = &topic_change;
	fcts[5] = &kick_user;
	fcts[6] = &invite_user;
	fcts[7] = &mode_change;
	fcts[8] = &channel_join;
	fcts[9] = &channel_leave;
	fcts[10] = &password_check;
	fcts[11] = &dcc;

	while (i < 11 && commands[i].compare(cmd) != 0)
		i++;
	if (i < 1)
	{
		iss >> arg;
		fcts[i](&arg, clientSockFd, irc_data);
	}
	else if (i == 1)
	{
		iss >> arg;
		fcts[i](&arg, clientSockFd, irc_data);
		if (*clientSockFd < 0)
			return ;
		std::getline(iss, args);
		set_user_infos(&args, clientSockFd, irc_data);
	}
	else if (i > 11)
		std::cerr << "command not found" << std::endl;
	else
		fcts[i](&str, clientSockFd, irc_data);
//	std::cerr << "DEBUG: execute_command ended successfully" << std::endl;
	return ;
}
