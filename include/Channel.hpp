/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/06 02:43:39 by aselnet          ###   ########.fr       */
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
		unsigned int			_usersLimit;
		std::list< User >	_usersList;
		std::list< User >	_operatorsList;
		std::list< User >	_invitedList;

	public:

		Channel( void );
		~Channel( void );
		Channel( const std::string name );

		void	setTopic( std::string topic );
		void	setKey( std::string key );
		void	setInviteMode( bool status );
		void	setTopicMode( bool status );
		void	setUsersLimit( unsigned int limit );
		
		std::string		getTopic( void ) const;
		std::string		getKey( void ) const;
		bool			getInviteMode( void ) const;
		bool			getTopicMode( void ) const;
		unsigned int	getUsersLimit( void ) const;
		
		void	addUser( User & user );
		void	kickUser( User & user );
		void	changeTopic( User & user, std::string & newTopic );

};

#endif