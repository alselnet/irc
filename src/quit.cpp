/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:55:30 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 13:05:15 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

void close_all(irc *irc_data, int epollFd, int serverSockFd) 
{
	std::list<User>::iterator it;

	for (it = irc_data->usersList.begin(); it != irc_data->usersList.end(); ++it)
		close(it->getSockFd());
	close(epollFd);
	close(serverSockFd);
	return ;
}