# DESCRIPTION

An IRC (Internet Relay Chat) server is a computer programme that enables real-time text-based chats over the Internet. Discord was originally an IRC server, as was Twitch Chat.


It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

The aim of ft_irc is to develop an IRC server in standard C++98, capable of handling multiple clients simultaneously.

## REQUIEREMENT

As client, you should be able to :
- authenticate, 
- set a nickname, 
- set a username, 
- join a channel,
- send and receive private messages 

On channel, you should have operators, regular users and commands that are specific to channel operators:
- Kick
- Invite
- Topic
- Mode (i, t, k, o, l)
  - i = invite only
  - t = topic
  - k = key (password)
  - o = operator
  - l = user limit

# INSTRUCTION

./ircserv <port> <password>

• port: The port number on which your IRC server will be listening for incoming IRC connections.
• password: The connection password. It will be needed by any IRC client that tries to connect to your server.

# RESSOURCES

https://medium.com/@mohamedsarda/ft-irc-channels-and-command-management-ff1ff3758a0b Tuto ft_irc about channel

https://modern.ircdocs.horse/ documentation IRC

