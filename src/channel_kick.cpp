// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_kick.cpp                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/19 13:15:39 by ctchen            #+#    #+#             //
//   Updated: 2024/02/19 15:46:24 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

template < typename T >
void	printContainer( T container )
{
	if (container.size() == 0)
	{
		std::cout << "Empty list !" << std::endl;
		return ;
	}
	
	typename T::const_iterator	it = container.begin();
	typename T::const_iterator	ite = container.end();

	for (; it != ite; it++)
	{
		std::cout << "[" << CYAN << it->getNickname() << RESET << "]" << std::endl;
	}
}

void	kick_user(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user((*clientSockFd), irc_data)->getNickname());
	std::list<User>::iterator		target = channel->findUserinCh(word_picker(str, 3));

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
	else if (target == channel->getUsersList().end())
	{
		Error ERR_USERNOTINCHANNEL(441, user->getNickname(), channel_name,
							   "The user you are trying to kick is not in the channel");
		ERR_USERNOTINCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else if (user == channel->getUsersList().end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else if (target != channel->getUsersList().end())
	{
		if (check_rights(user, channel) == true)
		{
			user->deleteChannel(channel_name);	
			Notif notif(user->getNickname() + "!" + user->getUsername() + "@"
						+ user->getIp(), "KICK", channel_name + " " + target->getNickname()
						, word_picker(str, 4));
			notif.to_client(*clientSockFd); // WIP => send to all
			notif.to_all_others(channel->getUsersList(), *clientSockFd);
			channel->getUsersList().erase(target);
			printContainer(channel->getUsersList());//check
		}
		else
		{
			Error ERR_NOTONCHANNEL(482, user->getNickname(), channel_name,
								   "You do not have operator privileges");
			ERR_NOTONCHANNEL.to_client(*clientSockFd);
		}
	}
}
