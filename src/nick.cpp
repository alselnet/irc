/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:47:53 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/22 20:52:49 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/Error.hpp"
#include "../include/Notif.hpp"

bool	erroneus(std::string nickname)
{
	std::string::iterator 	it;
	char					c;

	if (nickname.size() > 9)
		return (true);

	for (it = nickname.begin(); it != nickname.end(); ++it)
	 {
        c = *it;
        if ((c < 'A' || c > '}') && it == nickname.begin())
			return (true);
		if (it != nickname.begin())
		{
			if ((c < '0' || c > '9') && (c != '-') && (c < 'A' || c > '}'))
				return (true);
		}
    }
	return(false);
}

bool	duplicate_nick(std::string nickname, irc *irc_data)
{
	std::list<User>::iterator user;

	for (user = irc_data->usersList.begin(); user != irc_data->usersList.end(); user++)
	{
		if (!user->getNickname().empty() && user->getNickname().compare(nickname) == 0)
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
	return (false);
}
void	change_nick_everywhere(std::string oldnick, std::string newnick,irc *irc_data)
{
	std::list<Channel>::iterator chan;
	for (chan = irc_data->channelList.begin(); chan != irc_data->channelList.end(); chan++)
	{
		if (chan->findUserinCh(oldnick) != chan->getUsersListEnd())
		{
			chan->delUser(oldnick);
			chan->addUser(newnick);
		}
	}
	return ;
}

void nick(std::string *arg, int *clientSockFd, irc *irc_data)
{
	std::list<User>::iterator user;

	user = get_user((*clientSockFd), irc_data);
	if (nick_errorcheck((*arg), (*clientSockFd), irc_data))
	{
		std::cout << "Nickname error" << std::endl;
		return ;
	}
	if (user->getNickname().empty())
	{
		if (duplicate_nick(*arg, irc_data))
		{
			int	i = 1;
			std::stringstream ss;
			std::string nb;
			ss << i;
			nb = ss.str();
			while (duplicate_nick((*arg + "_" + nb), irc_data))
			{
				i++;
				ss.str(std::string());
				ss << i;
				nb = ss.str();
			}
			user->setNickname(*arg + "_" + nb);
		}
		else
			user->setNickname(*arg);
		handshake_replies((*clientSockFd), user->getNickname());
	}
	else if (duplicate_nick(*arg, irc_data))
	{
		Error	ERR_NICKNAMEINUSE(433, get_user(*clientSockFd, irc_data)->getNickname(), *arg, "Nickname is already in use");
		std::cout << ERR_NICKNAMEINUSE.get_reply_message() << std::endl;
		ERR_NICKNAMEINUSE.to_client(*clientSockFd);
		return ;
	}
	else
	{
		Notif	ACCEPTED (user->getNickname() + "!" + user->getUsername() + "@"
						  + user->getIp(), "NICK", (*arg), "");
		change_nick_everywhere(user->getNickname(), *arg, irc_data);
		user->setNickname(*arg);
		ACCEPTED.to_client(*clientSockFd);
		ACCEPTED.to_all_mates(*user, irc_data);
	}
	return ;
}
