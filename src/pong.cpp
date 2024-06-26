/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:46:02 by aselnet           #+#    #+#             */
//   Updated: 2024/02/19 00:39:18 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

void	pong(std::string *args, int *target_fd, irc *irc_data)
{
	(void) args;
	(void) irc_data;
	std::string pong_reply;

	pong_reply = ":" + SERVER_NAME + " PONG " + SERVER_NAME + "\r\n";
	send((*target_fd), pong_reply.c_str(), pong_reply.size(), 0);
	return ;
}
