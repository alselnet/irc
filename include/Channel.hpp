/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/22 22:19:10 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
	#define CHANNEL_HPP

#include "irc.hpp"
#include "User.hpp"

class Channel
{
	private:

		std::string	_name;
		std::string	_topic;
		std::string	_key;
		bool			_inviteMode;
		bool			_topicMode;
		int			_usersLimit;
		std::list< User >	_usersList;
		std::list< User >	_operatorsList;
		std::list< User >	_invitedList;

	public:

		Channel( void );
		~Channel( void );
		Channel( const std::string name );

		void	setTopic( const std::string topic );
		void	setKey( const std::string key );
		void	setInviteMode( const bool status );
		void	setTopicMode( const bool status );
		void	setUsersLimit( const int limit );
		void	addUser( const User & user );

};

#endif