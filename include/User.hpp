/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:57:37 by jthuysba          #+#    #+#             */
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
		bool			_irc_op;
		bool			_pass;
		int			_sockFd;
		struct sockaddr_in _serverAddr;
		std::list<std::string>	_channelList;	

	public:

		User( void );
		~User( void );
		User( const std::string & ip );
		User( const int sockFd );
		
		bool operator==( const User & rhs ) const;

		void	setNickname( const std::string & nickname );
		void	setUsername( const std::string & username );
		void	setRealname( const std::string & realname );
		void	setIp( const std::string & ip );
		void	setPass( const bool pass );
		void	setOperator(bool is_op);
		std::string getNickname( void ) const;
		std::string getUsername( void ) const;
		std::string getRealname( void ) const;
		std::string getIp( void ) const;
		int			getSockFd( void ) const;
		bool		getOperator() const;
		bool		getPass() const;
		std::list<std::string>	&getChannelList( void );
		void		addChannel( std::string & channel );
		void		deleteChannel( std::string & channel );
};

#endif
