/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:01:19 by ctchen            #+#    #+#             */
//   Updated: 2024/02/22 10:39:07 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

std::string	copy_remaining(std::string *str, unsigned int i)
{
	std::string	word = "";

	while (i < (*str).size())
	{
		word += (*str)[i];
		i++;
	}
	return (word);
}

bool	is_number(const std::string &str)
{
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (!std::isdigit(*it)) {
			return false;
		}
	}
	return true;
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
		if (limit == channel->getUsersLimit())
			return true;
	}
	return (false);
}

void	mode_channel(std::string *str, int *clientSockFd,
irc *irc_data, std::string *channel_name, std::list<User>::iterator user)
{
	std::list<Channel>::iterator	channel = get_channel((*channel_name), irc_data);
	std::string						flags = word_picker(str, 3);
	std::string						args = copy_remaining(str, index_to_word(str, 4));

	if (channel_name->empty() || flags.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	if (check_rights(user, channel) == true)
	{
		bool			set = 0;
		std::string	option = "";
		for (unsigned int i = 0; i < flags.size(); i++)
		{
			if (flags[0] != '+' && flags[0] != '-')
				return ;
			switch (flags[i])
			{
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
				if (key.empty() && set == true)
				{
					Error ERR_NEEDMOREPARAMS(461, user->getNickname(), (*channel_name), "k * You must specify a parameter for the key mode. Syntax: <key>.");
					ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
					break;
				}
				else if (channel->getKey().empty() == false && set == true)
				{
					Error ERR_KEYSET(467, user->getNickname(), (*channel_name), "The key is already set");
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
					Error ERR_USERNOTINCHANNEL(441, user->getNickname(), target + ":", "No such nick/channel");
					ERR_USERNOTINCHANNEL.to_client(*clientSockFd);
					break ;
				}
				option += "o " + target;
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
				if (limit.empty() && set == true)
				{
					Error no_limit(461, user->getNickname(), (*channel_name), "l * You must specify a parameter for the limit mode. Syntax: <limit>.");
					no_limit.to_client(*clientSockFd);
					break ;
				}
				if (set == 1 && (is_number(limit) == false || std::strtoul(limit.c_str(), NULL, 10) <= 0))
				{
					Error wrong_limit(461, user->getNickname() + (*channel_name) + "l ", limit, " Invalid limit mode parameter. Syntax: <limit>.");
					wrong_limit.to_client(*clientSockFd);
					break;
				}
				option += "l";
				if (set == 1)
				{
					option += " " + limit;
					channel->setUsersLimit(std::strtoul(limit.c_str(), NULL, 10));
				}
				else if (set == 0)
					channel->setUsersLimit(0);
				break;
			}
			default :
			{
				std::string single_flag(1, flags[i]);
				Error ERR_UNKNOWNMODE(472, user->getNickname(), single_flag, "");
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

void	mode_user(std::string *str, int *clientSockFd, std::string *nickname,
std::list<User>::iterator user)
{	
	std::string						active_umode = "+";
	std::string						modes = word_picker(str, 3);

	if (nickname->empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "MODE needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return;
	}
	else if ((*nickname) != user->getNickname())
	{
		Error ERR_USERSDONTMATCH(502, user->getNickname(), "",
								 "Can't change mode for other users");
		ERR_USERSDONTMATCH.to_client(*clientSockFd);
		return ;
	}
	else if (modes == "+o" || modes == "+O" || modes == "+oO" || modes == "+Oo"
		|| modes == "-o" || modes == "-O" || modes == "-oO" || modes == "-Oo")
	{
		Notif	forbidden(user->getNickname() + "!" + user->getUsername() + "@"
					  + user->getIp(), "Only server Operator can change this:", " forbidden", "");
		forbidden.to_client(*clientSockFd);
		return ;
	}
	else if (!modes.empty())
	{
		Error ERR_UMODEUNKNOWNFLAG(501, modes, "", ":Unknown MODE flag");
		ERR_UMODEUNKNOWNFLAG.to_client(*clientSockFd);
		return ;
	}
	if (user->getOperator())
		active_umode += "o";
	Reply RPL_UMODEIS(221, user->getNickname(), active_umode);
	RPL_UMODEIS.to_client(*clientSockFd);
}

void	mode_change(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string	second = word_picker(str, 2);
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);

	if (user == irc_data->usersList.end())
	{
		std::cerr << "Error: no user is impossible" << std::endl;
		return;
	}
	if (second.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Needs more parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	if (second[0] == '#')
		mode_channel(str, clientSockFd, irc_data, &second, user);
	else
		mode_user(str, clientSockFd, &second, user);
}
