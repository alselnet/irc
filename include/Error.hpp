/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 09:11:10 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/14 15:07:53 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <sstream>
#include "../include/irc.hpp"

class Error
{
	public:
			Error(int error_nb, std::string client_nick, std::string erroneous_parameter, std::string error_description);
			Error(Error &src);
			Error &operator=(Error &src);
			~Error(void);

			std::string	get_reply_message(void);
			const char*	get_cstr(void);	
			int			get_size(void);
			void		to_client(unsigned int target_fd);

	private:
			std::string _replyMessage;
			Error(void);
};

#endif