// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_moderator.cpp                              :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/19 10:00:15 by ctchen            #+#    #+#             //
//   Updated: 2024/02/19 10:19:52 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "irc.hpp"
#include "channel_command.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	channel_leave(std::string *str, int *clientSockFd, irc *irc_data)
{
//	std::cerr << "str =" << (*str) << std::endl;
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user((*clientSockFd), irc_data)->getNickname());

	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "JOIN needs parameter");
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
	else if (user == channel->getUsersList().end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else
		channel->delUser(user);
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "PART", channel_name, "");
	notif.to_client(*clientSockFd);
}

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
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "KICK", channel_name, "");
	notif.to_client(*clientSockFd);
}

void	invite_user(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::list<User>::iterator 		target = get_user_by_nick(word_picker(str, 2), irc_data);

	if (target == irc_data->usersList.end())
	{
		Error ERR_NOSUCHNICK(406, user->getNickname(), word_picker(str, 2) + " :"
							 , "There is no such nickname");
		ERR_NOSUCHNICK.to_client(*clientSockFd);
		return ;
	}
	else if (target == irc_data->usersList.end())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Invite needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	else if (user == channel->getUsersList().end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else if (channel->findUserinCh(word_picker(str, 2)) != channel->getUsersList().end())
	{
		Error ERR_USERONCHANNEL(443, user->getNickname(), word_picker(str, 2)
								+ " " + channel_name, "Target already in channel");
		ERR_USERONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else
	{
		if ((channel->getInviteMode() == true && check_rights(user, channel) == true)
			|| channel->getInviteMode() == false)
		{
			channel->addtoInviteList(target->getNickname());
			Notif	RPL_INVITING(user->getNickname() + "!" + user->getUsername() + "@"
								 + user->getIp(), "RPL_INVITING", target->getNickname(),
								 channel_name);
			RPL_INVITING.to_client(*clientSockFd);
		}
		else
		{
			Error ERR_CHANOPRIVSNEEDED(482, user->getNickname(), channel_name,
								   "You do not have operator privileges");
			ERR_CHANOPRIVSNEEDED.to_client(*clientSockFd);
		}
	}
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "INVITE", channel_name, "");
	notif.to_client(*clientSockFd);
}
