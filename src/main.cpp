/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:10:24 by aselnet           #+#    #+#             */
/*   Updated: 2024/01/31 17:30:56 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

int main( int argc, char **argv)
{
	(void) argc, (void) argv;

	// Channel	foo = Channel("Test");
	// User		ufoo = User("0.0.0.0");
	
	// ufoo.setNickname("Jules");
	// foo.addUser(ufoo);

	// User		vfoo = User("1.1.1.1");
	
	// vfoo.setNickname("Alex");
	// foo.addUser(vfoo);

	// foo.kickUser(vfoo);
	// foo.kickUser(vfoo);

	if (server_loop() < 0)
		return (-1);
	return (0);
}