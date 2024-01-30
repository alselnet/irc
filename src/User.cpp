/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:59:35 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/30 13:02:02 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/User.hpp"

/* Members Fucntions */

std::string User::getIp( void ) const
{
	return (_ip);
}

std::string User::getRealname( void ) const
{
	return (_realname);
}

std::string User::getUsername( void ) const
{
	return (_username);
}

std::string User::getNickname( void ) const
{
	return (_nickname);
}

void	User::setIp( const std::string & ip )
{
	_ip = ip;
}

void	User::setRealname( const std::string & realname )
{
	_realname = realname;
}

void	User::setUsername( const std::string & username )
{
	_username = username;
}

void	User::setNickname( const std::string & nickname )
{
	_nickname = nickname;
}

/* Operators Overload */

bool User::operator==( const User & rhs ) const
{
	return (this->_serverAddr.sin_addr.s_addr == rhs._serverAddr.sin_addr.s_addr);
}

/* Constr & Destr */

User::User( const std::string & nickname, const std::string & username, const std::string & realname, const std::string & ip) : _nickname(nickname), _username(username), _realname(realname), _ip(ip)
{
	std::cout << DARK_WHITE << "User : Complete Constructor" << END;
}

User::~User( void )
{
	std::cout << DARK_WHITE << "User : Destructor" << END;
}

User::User( const std::string & ip ) : _nickname(""), _username(""), _realname(""), _ip(ip)
{
	std::cout << DARK_WHITE << "User : IP Constructor" << END;
}

User::User( void ) : _nickname(""), _username(""), _realname(""), _ip("")
{
	std::cout << DARK_WHITE << "User : Void Constructor" << END;
}