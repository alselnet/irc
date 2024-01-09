/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:10:24 by aselnet           #+#    #+#             */
/*   Updated: 2024/01/09 12:25:11 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

int main( int argc, char **argv)
{
	(void) argc;
	(void) argv;
	
	int	socketServer = socket(AF_INET, SOCK_STREAM, 0);
	
	(void) socketServer;
	
	return (0);
}