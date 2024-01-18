/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/18 13:14:33 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
	#define CHANNEL_HPP

#include "irc.hpp"

class Channel
{
	private:

		std::string	_topic;
		std::string	_key;
		bool			_inviteMode;
		bool			_topicMode;
		int			_usersLimit;
		std::map< std::string, std::string >	_usersList;
		std::map< std::string, std::string >	_operatorsList;
		std::map< std::string, std::string >	_invitedList;

	public:

		Channel( void );
		~Channel();

};

#endif