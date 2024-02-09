// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_command.hpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/08 18:22:12 by ctchen            #+#    #+#             //
//   Updated: 2024/02/09 18:10:29 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef CHANNEL_COMMAND_HPP
# define CHANNEL_COMMAND_HPP

#include "irc.hpp"

void		kick_user(std::string str, int clientSockFd, irc *irc_data);
void		invite_user(std::string str, int clientSockFd, irc *irc_data);
void		mode_change(std::string str, int clientSockFd, irc *irc_data);
void		topic_change(std::string str, int clientSockFd, irc *irc_data);
void		channel_join(std::string chName, int clientSockFd, irc *irc_data);
void		channel_leave(std::string chName, int clientSockFd, irc *irc_data);

bool		check_rights(std::list<User>::const_iterator user,
						 std::list<Channel>::const_iterator chan);
std::string	word_picker(const std::string &str, unsigned int nb);
std::string word_skip_cut(std::string &str, unsigned long i);

#endif
