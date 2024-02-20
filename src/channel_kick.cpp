/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_kick.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:15:39 by ctchen            #+#    #+#             */
/*   Updated: 2024/02/20 11:01:47 by jthuysba         ###   ########.fr       */
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

void	boot_user(std::string *str, int *clientSockFd, irc *irc_data, std::string target_name)
{
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user((*clientSockFd), irc_data)->getNickname());
	std::list<User>::iterator		target = channel->findUserinCh(target_name);

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
	else if (target == channel->getUsersListEnd())
	{
		Error ERR_USERNOTINCHANNEL(441, user->getNickname(), channel_name,
							   "The user you are trying to kick is not in the channel");
		ERR_USERNOTINCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else if (user == channel->getUsersListEnd())
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
			Notif kicked(target->getNickname() + "!" + target->getUsername() + "@"
						 + target->getIp(), "PART", channel_name, "");
			kicked.to_client(target->getSockFd());
			Notif notif(user->getNickname() + "!" + user->getUsername() + "@"
						+ user->getIp(), "KICK", channel_name + " " + target->getNickname(), word_picker(str, 4));
			notif.to_client(*clientSockFd);
			notif.to_all_others(*channel, *clientSockFd);
			channel->eraseFromUserList(target);
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
	std::string		userlist = word_picker(str, 3);
	unsigned long	count_user = word_comma_replace(&userlist);

	for (unsigned long i = 0; i <= count_user; i++)
	{
		std::string select_user = word_picker(&userlist, i + 1);
		boot_user(str, clientSockFd, irc_data, select_user);
	}
}