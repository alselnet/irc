// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_parse.hpp                                  :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/19 10:23:53 by ctchen            #+#    #+#             //
//   Updated: 2024/02/21 14:06:11 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef CHANNEL_PARSE_HPP
#define CHANNEL_PARSE_HPP

unsigned long	index_to_word(std::string *str, unsigned long target);
std::string		word_extract(std::string &str);
unsigned long	word_comma_replace(std::string *str);
std::string		word_picker(const std::string *str, unsigned int nb);
bool			check_rights(std::list<User>::const_iterator user,
						 std::list<Channel>::const_iterator chan);

#endif
