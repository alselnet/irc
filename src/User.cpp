/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:59:35 by jthuysba          #+#    #+#             */
//   Updated: 2024/01/31 10:51:14 by ctchen           ###   ########.fr       //
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

bool		User::getOperator() const
{
	return (this->_irc_op);
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

void	User::setOperator(bool is_op)
{
	this->_irc_op = is_op
}

/* Operators Overload */

bool User::operator==( const User & rhs ) const
{
	return (this->_serverAddr.sin_addr.s_addr == rhs._serverAddr.sin_addr.s_addr);
}

/* Constr & Destr */

User::User( const std::string & nickname, const std::string & username, const std::string & realname, const std::string & ip) : _nickname(nickname), _username(username), _realname(realname), _ip(ip), irc_op(0)
{
	std::cout << DARK_WHITE << "User : Complete Constructor" << END;
}

User::User( const std::string & nickname, const std::string & username, const std::string & realname, const std::string & ip, bool is_op) : _nickname(nickname), _username(username), _realname(realname), _ip(ip), irc_op(is_op)
{
	std::cout << DARK_WHITE << "User : Complete Constructor+" << END;
}

User::~User( void )
{
	std::cout << DARK_WHITE << "User : Destructor" << END;
}

User::User( const std::string & ip ) : _nickname(""), _username(""), _realname(""), _ip(ip), _irc_op(0)
{
	std::cout << DARK_WHITE << "User : IP Constructor" << END;
}

User::User( void ) : _nickname(""), _username(""), _realname(""), _ip(""), _irc_op(0)
{
	std::cout << DARK_WHITE << "User : Void Constructor" << END;
}
