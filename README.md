# DESCRIPTION

An IRC (Internet Relay Chat) server is a computer programme that enables real-time text-based chats over the Internet. Discord was originally an IRC server, as was Twitch Chat.


It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

The aim of ft_irc is to develop an IRC server in standard C++98, capable of handling multiple clients simultaneously.

# REQUIEREMENT

As client, you should be able to :
- authenticate, 
- set a nickname, 
- set a username, 
- join a channel,
- send and receive private messages 

On channel, you should have operators, regular users and commands that are specific to channel Mode
  - i = invite only
  - t = topic
  - k = key (password)
  - o = operator
  - l = user limit

Bonus :
- Handle file transfer 
- A bot : conversation with ChatGPT and Weather

# INSTRUCTIONS

port : 12345 (but you can choose between 1024 and 65535)
password : you can choose

./ircserv <port> <password>

in another terminal
<nc ip <port> >
or 
irssi
/connect <ip> <port> <password>

Command :
PASS <password>
NICK <nickname>
USER <username> 0 * <realname>
JOIN <channel>{,<channel>} [<key>{,<key>}]
PRIVMSG <target>{,<target>} <text to be sent>
TOPIC <channel> [<topic>]
LIST [<channel>{,<channel>}] [<elistcond>{,<elistcond>}]
INVITE <nickname> <channel>
KICK <channel> <user> *( "," <user> ) [<comment>]
MODE <target> [<modestring> [<mode arguments>...]]

# RESSOURCES

https://medium.com/@mohamedsarda/ft-irc-channels-and-command-management-ff1ff3758a0b Tuto ft_irc about channel

https://modern.ircdocs.horse/ documentation IRC

