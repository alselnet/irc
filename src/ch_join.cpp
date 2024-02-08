/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ch_join.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 08:12:01 by tchen             #+#    #+#             */
/*   Updated: 2024/02/08 18:08:53 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	join_channel(std::string chName, int clientSockFd, irc *irc_data)
{
	if (get_channel(chName, irc_data) != irc_data->channelList.end())
	{
		get_channel(chName, irc_data)->addUser(get_user(clientSockFd, irc_data));
	}
	else
	{
		Channel	newchannel(chName);
		newchannel.addUser(get_user(clientSockFd, irc_data));
		newchannel.addOperator(get_user(clientSockFd, irc_data));
		irc_data->channelList.push_back(newchannel);
	}
}
