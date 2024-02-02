/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:57:37 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/02 17:33:11 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef	USER_HPP
	#define	USER_HPP
	
#include "irc.hpp"

class User
{
	private:

		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_ip;
		bool		_irc_op;
		struct sockaddr_in _serverAddr;

	public:

		User( void );
		~User( void );
		User( const std::string & ip );
		User( const std::string & nickName, const std::string & userName, const std::string & realname, const std::string & ip);
		User( const std::string & nickName, const std::string & userName, const std::string & realname, const std::string & ip, bool is_op);
		
		bool operator==( const User & rhs ) const;

		void	setNickname( const std::string & nickname );
		void	setUsername( const std::string & username );
		void	setRealname( const std::string & realname );
		void	setIp( const std::string & ip );
		void	setOperator(bool is_op);
		std::string getNickname( void ) const;
		std::string getUsername( void ) const;
		std::string getRealname( void ) const;
		std::string getIp( void ) const;
		bool		getOperator() const;
};

#endif
