/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:01:19 by ctchen            #+#    #+#             */
//   Updated: 2024/02/20 17:20:50 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

//MODE +i t | marche
//MODE t +i | marche pas
//MODE +k t | +k t | t as key
//MODE +k mdr t | if key alr set then do nothing unless new flags
std::string	get_args(std::string *str, unsigned int i)
{//+i t +k -i +l mdp 10
	std::string	args = "";

	if ((*str).empty() == false && ((*str)[i] != '+' && (*str)[i] != '-'))
		args = word_extract(*str, i);
	return (args);
}

std::string	active_mode(std::list<Channel>::iterator channel)
{
	std::string	active = "";

	if (channel->getInviteMode())
		active += "i";
	if (channel->getTopicMode())
		active += "t";
	if (!channel->getKey().empty())
		active += "k";
	if (!channel->operatorsListEmpty())
		active += "o";
	if (channel->getUsersLimit() > 0)
		active += "l";
	if (active.size() > 0)
		return ("+" + active);
	return (active);
}

bool	check_set(char option, bool set, std::list<Channel>::iterator channel,
				  std::string arg)
{
	if (option == 'i')
	{
		if ((channel->getInviteMode() == true && set == true)
			|| (channel->getInviteMode() == false && set == false))
			return true;
	}
	else if (option == 't')
	{
		if ((channel->getTopicMode() == true && set == true)
			|| (channel->getTopicMode() == false && set == false))
			return true;
	}
	else if (option == 'k')
	{
		if (set == true && channel->getKey().empty() == false)
			return true;
	}
	else if (option == 'o')
	{
		if (set == true && channel->findOpinCh(arg) != channel->getOpListEnd())
			return true;
	}
	else if (option == 'l')
	{
		unsigned long limit = std::strtoul(arg.c_str(), NULL, 10);
		if ((set == true && limit > 0 &&
			 limit != channel->getUsersLimit())
			|| set == false)
			return true;
	}
	return (false);
}

/*
void	invite_list(std::list<User>::iterator user, int * clientSockFd,
					std::string *channel_name,
					std::list<Channel>::iterator channel)
{
	std::list<std::string>::const_iterator invited = channel->getInvitedListBegin();
	std::list<std::string>::const_iterator inviter = channel->getInviterListBegin();

	while (invited != channel->getInvitedListEnd())
	{
		Notif RPL_INVITELIST(user->getNickname() + "!" + user->getUsername() + "@" + user->getIp(), "346", (*invited) + " " + (*channel_name), (*inviter));
		RPL_INVITELIST.to_client(*clientSockFd);
		invited++;
		inviter++;
	}
	Notif RPL_ENDOFINVITELIST(user->getNickname() + "!" + user->getUsername() + "@" + user->getIp(), "347", (*invited) + " " + (*channel_name), "");
	RPL_ENDOFINVITELIST.to_client(*clientSockFd);
}
*/

void	mode_channel(std::string *str, int *clientSockFd, irc *irc_data,
					 std::string *channel_name)
{
	std::list<Channel>::iterator	channel = get_channel((*channel_name), irc_data);
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::string						flags = word_picker(str, 3);
	std::string						args = get_args(str, index_to_word(str, 4));

	if (channel_name->empty())//if param don't match options
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), (*channel_name),
								 "Not enough parameters");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	if (flags.empty())
	{//a pas l'air de marcher
		Notif notif(SERVER_NAME, "324", user->getNickname()  + " #" +
					(*channel_name), active_mode(channel));
		notif.to_client(*clientSockFd);
		return ;
	}
	if (check_rights(user, channel) == true || user->getOperator() == 1)
	{
		bool		set = 0;
		std::string	option = "";
		for (unsigned int i = 0; i < flags.size(); i++)
		{
			switch (flags[i])
			{
			case ' ':
				break;
			case '+':
			{
				option += '+';
				set = 1;
				break;
			}
			case '-':
			{
				option += '-';
				set = 0;
				break;
			}
			case 'i':
			{
				if (check_set('i', set, channel, "") == true)
					break;
				option += 'i';
				//invite_list(user, clientSockFd, channel_name, channel);
				channel->setInviteMode(set);
				break;
			}
			case 't':
			{
				if (check_set('t', set, channel, "") == true)
					break;
				option += 't';
				channel->setTopicMode(set);
				break;
			}
			case 'k':
			{
				std::string key = word_extract(args);
				if (channel->getKey().empty() == false && set == 1)
				{
					Error ERR_KEYSET(467, user->getNickname(), (*channel_name),
									 "The key is already set");
					ERR_KEYSET.to_client(*clientSockFd);
					break;
				}
				if (check_set('k', set, channel, key) == true)
					break;
				option += "k " + key;
				if (set == 1)
					channel->setKey(key);
				else if (set == 0)
					channel->delKey();
				break;
			}
			case 'o':
			{
				std::string target = word_extract(args);
				if (target.empty())
					break;
				if (check_set('o', set, channel, target) == true)
					break;
				if (channel->findUserinCh(target) == channel->getUsersListEnd())
				{
					Error ERR_USERNOTINCHANNEL(441, target, (*channel_name),
											   "User is not in the channel");
					ERR_USERNOTINCHANNEL.to_client(*clientSockFd);
					break ;
				}
				option += 'o';
				if (set == 1)
					channel->addOperator(target);
				else if (set == 0)
					channel->delOperator(target);
				break;
			}
			case 'l':
			{
				std::string limit = word_extract(args);
				if (check_set('l', set, channel, limit) == true)
					break ;
				option += "l " + limit;
				std::cout << option << std::endl;
				if (set == 1)
					channel->setUsersLimit(std::strtoul(limit.c_str(), NULL, 10));
				else if (set == 0)
					channel->setUsersLimit(0);
				break;
			}
			default :
			{
				Error ERR_UNKNOWNMODE(472, user->getNickname(), (*channel_name),
									  "Unknown mode");
				ERR_UNKNOWNMODE.to_client(*clientSockFd);
				break ;
			}
			}
		}
		if (option == "+" || option == "-")
			return ;
		Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
					  + user->getIp(), "MODE", (*channel_name), option);
		notif.to_client(*clientSockFd);
		notif.to_all_others(*channel, *clientSockFd);
	}
	else if (check_rights(user, channel) == false)
	{
		Error ERR_CHANOPRIVSNEEDED(482, user->getNickname(), (*channel_name),
					"You do not have operator privileges");
		ERR_CHANOPRIVSNEEDED.to_client(*clientSockFd);
	}
}

void	mode_user(std::string *str, int *clientSockFd, irc *irc_data, std::string *nickname)
{
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::string						active_umode = "+";
	std::string						modes = word_picker(str, 3);

//	active_umode += "w";
	if (nickname->empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "MODE needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return;
	}
	else if ((*nickname) != user->getNickname())
	{//can change if serv op ?
		Error ERR_USERSDONTMATCH(502, user->getNickname(), "",
								 "Can't change mode for other users");
		ERR_USERSDONTMATCH.to_client(*clientSockFd);
		return ;
	}
	//ERR_UMODEUNKNOWNFLAG = 501
	if (user->getOperator())
		active_umode += "o";
	Reply RPL_UMODEIS(221, user->getNickname(), active_umode);
	RPL_UMODEIS.to_client(*clientSockFd);
}

void	mode_change(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string	second = word_picker(str, 2);

	if (second[0] == '#')
		mode_channel(str, clientSockFd, irc_data, &second);
	else
		mode_user(str, clientSockFd, irc_data, &second);
}
