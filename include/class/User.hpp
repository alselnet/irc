/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:57:37 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/18 15:06:28 by jthuysba         ###   ########.fr       */
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

	public:

		User( void );
		~User();
		User( const std::string & ip );

		void	setNickname( const std::string & nickname );
		void	setUsername( const std::string & username );

};

#endif