// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ch_join.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/07 8:12:01 by ctchen             #+#    #+#             //
//   Updated: 2024/02/08 12:03:12 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

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
