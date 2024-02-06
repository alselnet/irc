/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:55:30 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 04:55:54 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

void close_all(int *clientFds, int epollFd, int serverSockFd, int clientNb) 
{
    for (int i = 0; i < clientNb; ++i) 
	{
		close(clientFds[i]);
    }
	close(epollFd);
	close(serverSockFd);
	return ;
}