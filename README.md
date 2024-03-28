ft_irc

--------------------------------
This is a basic CPP IRC server, by a team of three students of 42 School, Paris : 

C. Tchen (https://github.com/ctchen42)

J. Thuysbaert (https://github.com/Joule444)

myself, A. Selnet (https://github.com/alselnet)

The goal, as set by the subject, was to design a small IRC server working with any IRC client of our choice. As such, we have intended our server to work with the Unix-based client Irssi, that we cherry-picked for its 
abunding available documentation.  The server was implemented with the following functionalities :

- Basic channel commands (join, leave, topic, ...)
- Private Messaging
- Channel operator commands, such as KICK, or through the MODE command (join restrictions - by invitation or password -, possibility to set-up a max user capacity on any channel, transfer of operator rights)

--------------------------------
Quick How-to-use guide :

Server-side :

- clone our Repo
- make
- ./bin/ircserv 6667 "password"
(6667 is the usual IRC port number)

Client-side :
  
 - Download irssi here https://irssi.org/download/
 - /irssi
 - /set nick "your_nickname"
 - /connect localhost 6667 "password"
 - you're in !

---------------------------------
Some of the useful resource we used :

Beej's guide to Network Programming : https://beej.us/guide/bgnet/html/

RFC 2812 (IRC protocol) : https://www.rfc-editor.org/rfc/rfc2812

RFC 793 (TCP protocol) : https://www.rfc-editor.org/rfc/rfc793

IRC Protocol exhaustive documentation : https://dd.ircdocs.horse/ 

Irssi's documentation : https://irssi.org/documentation

