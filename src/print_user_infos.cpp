/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_user_infos.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:08:34 by aselnet           #+#    #+#             */
//   Updated: 2024/02/22 09:46:46 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

void	print_user_infos(std::list<User>::iterator &user)
{
	std::cout << "SAVED USER INFO" << std::endl;
	std::cout << "nickname: " << user->getNickname() << std::endl;
	std::cout << "username: " << user->getUsername() << std::endl;
	std::cout << "hostname: " << user->getIp() << std::endl;
	std::cout << "realname: " << user->getRealname() << std::endl << std::endl;
	return ;
}
