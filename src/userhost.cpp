/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:53:23 by aselnet           #+#    #+#             */
//   Updated: 2024/02/19 00:35:13 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/Error.hpp"

bool	need_more_params(std::string username)
{
	std::string::iterator 	it;
	char					c;

	if (username.empty())
		return (true);
	for (it = username.begin(); it != username.end(); ++it)
	 {
        c = *it;
        if (c == ' ' || c == '\r' || c == '\n' || c == '@' || !c)
			return (true);
    }
	return(false);
}

bool	duplicate_user(std::string username, irc *irc_data)
{
	std::list<User>::iterator user;

	for (user = irc_data->usersList.begin(); user != irc_data->usersList.end(); user++)
	{
		if (user->getUsername().compare(username) != 0)
			return (true);
	}
	return (false);
}

bool	user_errorcheck(std::string username, int clientSockFd, irc *irc_data)
{

	if (need_more_params(username))
	{
		Error	ERR_NEEDMOREPARAMS(461, get_user(clientSockFd, irc_data)->getNickname(), "userhost", "Not enough parameters");
		ERR_NEEDMOREPARAMS.to_client(clientSockFd);
		return (true);
	}
	// else if (duplicate_user(username, irc_data))
	// {
	// 	Error	ERR_ALREADYREGISTERED(462, get_user(clientSockFd, irc_data)->getNickname(), username, "Unauthorized command (already registered)");
	// 	ERR_ALREADYREGISTERED.to_client(clientSockFd);
	// 	return (true);
	// }
	return (false);
}

void userhost(std::string *arg, int *clientSockFd, irc *irc_data)
{
	get_user((*clientSockFd), irc_data)->setUsername(*arg);
	return ;
}
