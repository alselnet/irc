/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:31:35 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/22 16:56:36 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

std::list<User>::iterator get_user_by_nick( std::string nickname, irc * irc_data )
{
	std::list<User>::iterator	it = irc_data->usersList.begin();
	std::list<User>::iterator	ite = irc_data->usersList.end();

	for (; it != ite; it++)
	{
		if (it->getNickname() == nickname)
			return (it);
	}
	return (ite);
}

std::list<User>::iterator get_user( int clientSockFd, irc * irc_data )
{
	std::list<User>::iterator	it = irc_data->usersList.begin();
	std::list<User>::iterator	ite = irc_data->usersList.end();

	for (; it != ite; it++)
	{
		if (it->getSockFd() == clientSockFd)
			return (it);
	}
	return (ite);
}

std::list<Channel>::iterator get_channel(std::string chan_name, irc *irc_data )
{
	if (irc_data->channelList.size() == 0)
		return (irc_data->channelList.end());
	
	std::list<Channel>::iterator	it = irc_data->channelList.begin();
	std::list<Channel>::iterator	ite = irc_data->channelList.end();

	for (; it != ite; it++)
	{
		if (it->getChName() == chan_name)
		{
			return (it);
		}
	}
	return (ite);
}
