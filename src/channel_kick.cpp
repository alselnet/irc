/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_kick.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:15:39 by ctchen            #+#    #+#             */
//   Updated: 2024/02/22 17:33:34 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	printUsersList( std::list<User> & list )
{
	if (list.size() == 0)
	{
		std::cout << "Empty list !" << std::endl;
		return ;
	}
	
	std::list<User>::iterator it = list.begin();
	std::list<User>::iterator ite = list.end();

	for (; it != ite; it++)
	{
		std::cout << "[" << CYAN << it->getNickname() << RESET << "]" << std::endl;
	}
}

void	boot_user(std::string *str, int *clientSockFd, irc *irc_data, std::string target_name,
std::list<User>::iterator user)
{
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);

	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Kick needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	else if (channel == irc_data->channelList.end())
	{
		Error ERR_NOSUCHCHANNEL(403, user->getNickname(), channel_name,
								"There is no such channel");
		ERR_NOSUCHCHANNEL.to_client(*clientSockFd);
		return ;
	}
	std::list<std::string>::iterator		target = channel->findUserinCh(target_name);
	if (target == channel->getUsersListEnd())
	{
		Error ERR_USERNOTINCHANNEL(441, user->getNickname(), channel_name,
							   "The user you are trying to kick is not in the channel");
		ERR_USERNOTINCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else if (channel->findUserinCh(user->getNickname()) == channel->getUsersListEnd())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else if (target != channel->getUsersListEnd())
	{
		if (check_rights(user, channel) == true)
		{
			user->deleteChannel(channel_name);
			Notif kicked(user->getNickname() + "!" + user->getUsername() + "@"
						 + user->getIp(), "PART", channel_name, "");
			kicked.to_client(user->getSockFd());
			Notif notif(user->getNickname() + "!" + user->getUsername() + "@"
						+ user->getIp(), "KICK", channel_name + " " + user->getNickname(), word_picker(str, 4));
			notif.to_client(*clientSockFd);
			notif.to_all_others(*channel, *clientSockFd, irc_data->usersList);
			channel->delUser(target_name);
		}
		else
		{
			Error ERR_NOTONCHANNEL(482, user->getNickname(), channel_name,
								   "You do not have operator privileges");
			ERR_NOTONCHANNEL.to_client(*clientSockFd);
		}
	}
}

void	kick_user(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::string		userlist = word_picker(str, 3);
	unsigned long	count_user = word_comma_replace(&userlist);

	if (word_picker(str, 2).empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Kick needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	for (unsigned long i = 0; i <= count_user; i++)
	{
		std::string select_user = word_picker(&userlist, i + 1);
		boot_user(str, clientSockFd, irc_data, select_user, user);
	}
}
