/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/02 12:11:03 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
	#define CHANNEL_HPP

#include "irc.hpp"

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
		
		void		addUser( User & user, User & init );
		void		kickUser( User & user, User & init );
		void		delKey();
		void		addOperator(std::string username);
		void		delOperator(std::string username);
		void		modeChange(User &init, std::string str);
		bool		checkRights(User &init) const;
		std::string wordRemoveExtract(std::string &str, unsigned long i);
		std::string	firstWord(std::string str);
		User    	findUsername(std::string username);
		bool    	commandHandler(User &init, std::string &str);
};

#endif
