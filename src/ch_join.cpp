// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ch_join.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/07 8:12:01 by ctchen             #+#    #+#             //
//   Updated: 2024/02/07 10:32:06 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

bool	check_channel(irc *irc_data, std::string chName)
{
}
	
void	join_channel(irc *irc_data, std::string chName)
{
	if (check_channel(irc_data, chName) == 1)
	{
		irc_data->channelList;
	}
	else
	{
		Channel	newchannel(chName);
		newchannel.addOperator();
		irc_data->channelList.push_back(newchannel);
	}
}
