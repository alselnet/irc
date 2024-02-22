/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:23:05 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/22 16:35:19 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/channel_command.hpp"
# include "../include/Error.hpp"

typedef void	(*command_ptr)(std::string *arg, int *clientSockFd, irc *irc_data);

void	execute_command(std::string str, int *clientSockFd, irc *irc_data)
{
	std::string			arg;
	std::string			args;
	std::string			cmd;
	std::istringstream	iss(str);
	std::string			commands[11];
	command_ptr			fcts[11];
	int 				i;

	std::list<User>::iterator	user = get_user(*clientSockFd, irc_data);
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

	while (i < 11 && commands[i].compare(cmd) != 0)
		i++;
	
	if (i != 10 && cmd.compare("CAP") && user->getPass() == false)
	{
		Error	ERR_NOLOGIN(464, get_user(*clientSockFd, irc_data)->getNickname(), "", "Password required");
		std::cout << "Missing Password" << std::endl;
		std::cout << "Closing the connection..." << std::endl;
		ERR_NOLOGIN.to_client(*clientSockFd);
		delete_user(*clientSockFd, irc_data);
		*clientSockFd = -1;
		return ;
	}
	if ((i > 1 && i < 10) && (user->getNickname().empty() || user->getUsername().empty()
			|| user->getIp().empty()))
	{
		std::string not_found = "Nickname, Username or Hostname not set";
		std::cout << not_found << std::endl << "Closing the connection..." << std::endl;
		send(*clientSockFd, not_found.c_str(), not_found.size(), 0);
		delete_user(*clientSockFd, irc_data);
		*clientSockFd = -1;
		return ;
	}
	
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
	else if (i > 10)
		std::cerr << "command not found" << std::endl;
	else
	{
		fcts[i](&str, clientSockFd, irc_data);
	}
	return ;
}
