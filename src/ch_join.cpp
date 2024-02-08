/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ch_join.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:12:01 by tchen             #+#    #+#             */
/*   Updated: 2024/02/08 16:33:29 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	join_channel(std::string chName, int clientSockFd, irc *irc_data)
{
	if (getChannel(chName, irc_data) != irc_data->channelList.end())
	{
		getChannel(chName, irc_data)->addUser(getUser(clientSockFd, irc_data));
	}
	else
	{
		Channel	newchannel(chName);
		newchannel.addUser(getUser(clientSockFd, irc_data));
		newchannel.addOperator(getUser(clientSockFd, irc_data));
		irc_data->channelList.push_back(newchannel);
	}
}
