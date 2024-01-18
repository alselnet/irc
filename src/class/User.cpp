/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:59:35 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/18 15:06:46 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "User.hpp"

/* Members Fucntions */

void	User::setUsername( const std::string & username )
{
	_username = username;
}

void	User::setNickname( const std::string & nickname )
{
	_nickname = nickname;
}

/* Constr & Destr */

User::User( const std::string & ip ) : _ip(ip), _nickname(""), _username("")
{
	std::cout << DARK_WHITE << "User : IP Constructor" << END;
}

User::User( void ) : _ip(""), _nickname(""), _username("")
{
	std::cout << DARK_WHITE << "User : Void Constructor" << END;
}