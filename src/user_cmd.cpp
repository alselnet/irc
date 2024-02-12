/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:53:23 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/12 17:30:13 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

void user(std::string arg, int clientSockFd, irc *irc_data)
{
	get_user(clientSockFd, irc_data)->setUsername(arg);
	return ;
}