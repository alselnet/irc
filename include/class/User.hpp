/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 12:57:37 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/18 13:15:09 by jthuysba         ###   ########.fr       */
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

	public:

		User( void );
		~User();

};

#endif