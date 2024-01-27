/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:10:24 by aselnet           #+#    #+#             */
/*   Updated: 2024/01/23 15:46:14 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

int main( int argc, char **argv)
{
	(void) argc, (void) argv;

	Channel	foo = Channel("Test");
	User		ufoo = User("0.0.0.0");
	
	ufoo.setNickname("Jules");
	foo.addUser(ufoo);

	User		vfoo = User("1.1.1.1");
	
	vfoo.setNickname("Alex");
	foo.addUser(vfoo);

	foo.kickUser(vfoo);
	foo.kickUser(vfoo);

	return (0);
}