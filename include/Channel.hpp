/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/07 19:01:09 by ctchen           ###   ########.fr       //
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

		void						setChName( std::string name );
		void						setTopic( std::string topic );
		void						setKey( std::string key );
		void						setInviteMode( bool status );
		void						setTopicMode( bool status );
		void						setUsersLimit( unsigned int limit );

		std::list< User >			getOperatorsList(void) const;
		std::string					getTopic( void ) const;
		std::string					getKey( void ) const;
		std::string					getChName( void) const;
		bool						getInviteMode( void ) const;
		bool						getTopicMode( void ) const;
		unsigned int				getUsersLimit( void ) const;

		void						addUser( User & user);
		void						inviteUser( std::list<User>::const_iterator user,
												std::string target, bool is_op );
//		void						kickUser( std::list<User>::const_iterator user, bool is_op );
		void						kickUser( std::string username, bool is_op );
		void						delKey();
		void						addOperator(std::list<User>::const_iterator user);
		void						delOperator(std::list<User>::const_iterator user);
		void						modeChange(std::list<User>::const_iterator user,
									   std::string str, bool is_op);
		std::string 				wordRemoveExtract(std::string &str, unsigned long i);
		std::string					firstWord(std::string str);
		void						changeTopic( std::string nickname,
												 std::string & newTopic, bool is_op );
		void						modeMsg(const char *word, bool set, char flag,
											std::string username );
		std::list<User>::iterator	findUserinCh(std::string username);
};

#endif
