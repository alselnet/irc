/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:47:53 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/14 15:37:47 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/Error.hpp"
#include "../include/Notif.hpp"

bool	erroneus(std::string nickname)
{
	std::string::iterator it;
	std::string::iterator next;

	if (nickname.size() > 9)
		return (true);

	next = nickname.begin();
	next++;
	for (it = nickname.begin(); it != nickname.end(); ++it)
	 {
		if (next != nickname.end())
			next++;
        char c = *it;
        if ((c < 'A' || c > '}') && next != nickname.end())
			return (true);
		if (next == nickname.end())
		{
			if ((c < '0' || c > '9') && (c != '-') && (c < 'A' || c > '}'))
				return (true);
		}
    }
	return(false);
}

bool	duplicate(std::string nickname, irc *irc_data)
{
	std::list<User>::iterator user;

	for (user = irc_data->usersList.begin(); user != irc_data->usersList.end(); user++)
	{
		if (user->getNickname().compare(nickname) != 0)
			return (true);
	}
	return (false);
}

bool	nick_errorcheck(std::string nickname, int clientSockFd, irc *irc_data)
{

	if (nickname.empty())
	{
		Error	ERR_NONICKNAMEGIVEN(431, get_user(clientSockFd, irc_data)->getNickname(), "", "No nickname given");
		ERR_NONICKNAMEGIVEN.to_client(clientSockFd);
		return (true);
	}
	else if (erroneus(nickname))
	{
		Error	ERR_ERRONEUSNICKNAME(432, get_user(clientSockFd, irc_data)->getNickname(), nickname, "Erroneus nickname");
		ERR_ERRONEUSNICKNAME.to_client(clientSockFd);
		return (true);
	}
	// else if (duplicate(nickname, irc_data))
	// {
	// 	Error	ERR_NICKNAMEINUSE(433, get_user(clientSockFd, irc_data)->getNickname(), nickname, "Nickname is already in use");
	// 	ERR_NICKNAMEINUSE.to_client(clientSockFd);
	// 	return (true);
	// }
	return (false);
}

void nick(std::string arg, int clientSockFd, irc *irc_data)
{
	std::list<User>::iterator user;

	user = get_user(clientSockFd, irc_data);
	if (nick_errorcheck(arg, clientSockFd, irc_data))
		return ;	
	if (user->getNickname().empty())
	{
		user->setNickname(arg);
		handshake_replies(clientSockFd, user->getNickname());
	}
	else
	{
		Notif	ACCEPTED (user->getNickname() + "!" + user->getUsername() + "@"
 + user->getIp(), "NICK", arg, "");
		user->setNickname(arg);
		ACCEPTED.to_client(clientSockFd);
		//ACCEPTED.to_all() notifier dans tous les channels ou le client est present son changement de nickname
	}
	return ;
}
