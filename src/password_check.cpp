/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   password_check.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 18:32:49 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/22 16:34:01 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Error.hpp"

void password_check(std::string *arg, int *clientSockFd, irc *irc_data)
{
	std::stringstream	iss(*arg);;
	std::string			dump;
	std::string			pass;

	iss >> dump;
	iss >> pass;

	Error	ERR_PASSWDMISMATCH(464, get_user(*clientSockFd, irc_data)->getNickname(), "", "Password incorrect");
	if (irc_data->password.compare(pass) != 0)
	{
		std::cout << "Incorrect password" << std::endl;
		std::cout << "Closing  the connection..." << std::endl;
		ERR_PASSWDMISMATCH.to_client(*clientSockFd);
		delete_user(*clientSockFd, irc_data);
		*clientSockFd = -1;
	}
	else
		get_user(*clientSockFd, irc_data)->setPass(true);
	return ;
}
