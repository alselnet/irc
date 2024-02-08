/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notif.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 03:53:56 by aselnet           #+#    #+#             */
//   Updated: 2024/02/08 21:47:30 by ctchen           ###   ########.fr       //
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTIF_HPP
#define NOTIF_HPP

#include <string>
#include <sstream>
#include "../include/irc.hpp"

class Notif
{
	public:
			Notif(std::string id_string, std::string command, std::string arguments, std::string additional);
			Notif(Notif &src);
			Notif &operator=(Notif &src);
			~Notif(void);

			std::string	get_notif_message(void);
			const char*	get_cstr(void);	
			int			get_size(void);
			void			to_all_others(std::list<User> target_fds, int originFd);
			void			to_client(unsigned int target_fd);

	private:
			std::string _notifMessage;

			Notif(void);
};

#endif
