/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:55:30 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 15:16:29 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

void	delete_user( int sockFd, irc * irc_data )
{
	std::list<User>::iterator	it = irc_data->usersList.begin();
	std::list<User>::iterator	ite = irc_data->usersList.end();

	for (; it != ite; it++)
	{
		if (it->getSockFd() == sockFd)
		{
			irc_data->usersList.erase(it);
			break ;
		}
	}
	close(sockFd);
}

void	close_all(irc *irc_data, int epollFd, int serverSockFd) 
{
	std::list<User>::iterator it;

	for (it = irc_data->usersList.begin(); it != irc_data->usersList.end(); ++it)
		close(it->getSockFd());
	close(epollFd);
	close(serverSockFd);
	return ;
}