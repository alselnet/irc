/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:57:37 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/22 19:56:48 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	USER_HPP
	#define	USER_HPP
	
#include "irc.hpp"

class User
{
	private:

		std::string	_ip;
		std::string	_nickname;
		std::string	_username;
		
		struct sockaddr_in _serverAddr;

	public:

		User( void );
		~User( void );
		User( const std::string & ip );
		
		bool operator==( const User & rhs ) const;

		void	setNickname( const std::string & nickname );
		void	setUsername( const std::string & username );
		std::string getNickname( void ) const;

};

#endif