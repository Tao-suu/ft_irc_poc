# DESCRIPTION

## Definition
ft_irc is an IRC (Internet Relay Chat) server written in C++98.

IRC is a protocol that enables real-time text communication over the Internet. Users connect to an IRC server using an IRC client and can communicate through public channels or private messages.

An IRC network can contain multiple connected servers, while each server can handle multiple clients simultaneously.

The goal of this project is to implement an IRC server capable of handling multiple clients at the same time, following the IRC protocol and supporting common IRC commands and channel management features.

## Requierements

The server must support the following client functionality:

- Authentication with a server password
- Nickname registration
- Username registration
- Joining channels
- Leaving channels
- Public channel messages
- Private messages
- Channel operators
- Channel modes
- Channel invitations
- Kicking users
- Changing channel topics

Bonus : File transfert and bot

# INSTRUCTIONS

Start the IRC server with:
The port must be between 1024 and 65535.
The password is chosen by the server operator and is required by clients when connecting.

1) Launch server
./ircserv <port> <password>
For example:
./ircserv 12345 mypassword

2) Connecting server
Using Netcat from another terminal:
nc <ip> <port>
For example:
nc 127.0.0.1 12345

or

Using Irssi
irssi
/connect <ip> <port> <password>
For example:
/connect 127.0.0.1 12345 mypassword

3) Client Authentication

Before using most IRC commands, the client must register with the server.

- PASS
Provides the password required by the server, it must match the password used when starting the server.
PASS <password>

- NICK
Sets the client's nickname, which identifies the client on the IRC server and must be unique.
NICK <nickname>


- USER
Registers the username and real name of the client, they are used by the server to identify the client.
USER <username> <mode> <unused> <realname>
Example:
USER alice 0 * Alice Smith


After successful authentication, the client can join channels and communicate with other users.

4) IRC COMMANDS

- JOIN
Joins one or more IRC channels.
JOIN <channel>{,<channel>} [<key>{,<key>}]
Example:
JOIN #general
Multiple channels can be joined at once:
JOIN #general,#gaming

A channel password can be provided when joining a channel protected by +k:
JOIN #private secret123
For multiple channels with different keys:
JOIN #general,#private key1,key2

- PRIVMSG
Sends a private message to another user or a message to a channel.
PRIVMSG <target>{,<target>} <text to be sent>
PRIVMSG alice Hello Alice!
Message to a channel
PRIVMSG #general Hello everyone!
Multiple targets
PRIVMSG alice,bob Hello!

- TOPIC
Displays or changes the topic of a channel.
TOPIC <channel>
Displays the current topic:
TOPIC #general
To change the topic:
TOPIC <channel> <topic>
When topic protection (+t) is enabled, only channel operators are allowed to change the topic.

- LIST
Displays available channels.
You can also request information about specific channels:
LIST <channel>
Example:
LIST #general
Multiple channels can be requested:
LIST #general,#gaming
The server can return information such as the channel name, number of users, and topic.

- INVITE
Invites a user to a channel.
INVITE <nickname> <channel>
Example:
INVITE alice #private
The invited user can then join the channel, even when the channel has invite-only mode (+i) enabled, subject to the server's implementation.Typically, the command is used by a channel operator.

- PART
Leaves one or more channels.
PART <channel>{,<channel>}
Example:
PART #general
Multiple channels can be left at once:
PART #general,#gaming
A reason can optionally be provided:
PART #general Goodbye everyone!

- KICK
Removes one or more users from a channel.
KICK <channel> <user> *( "," <user> ) [<comment>]
Example:
KICK #general alice
A reason can be provided:
KICK #general alice Spamming
Multiple users can be kicked:
KICK #general alice,bob
This command requires appropriate channel operator privileges.

- MODE
The MODE command is used to view or modify channel settings.
MODE <target> [<modestring> [<mode arguments>...]]
For channels, the supported modes are:

Mode	Name	Description
+i	Invite-only	Only invited users can join the channel
+t	Topic protection	Only operators can change the topic
+k	Channel key	A password is required to join the channel
+o	Operator	Gives or removes operator privileges
+l	User limit	Sets the maximum number of users
Mode i — Invite Only

- BOT WEATHER
Requests weather information for a city.
BOT WEATHER <city>
Example:
BOT WEATHER Paris


- BOT CHATGPT
Sends a message to the ChatGPT-powered bot.
BOT CHATGPT <text>
Example:
BOT CHATGPT Explain how IRC channels work

# RESOURCES
IRC Documentation

Modern IRC documentation:
"https://modern.ircdocs.horse/?utm_source=chatgpt.com"

ft_irc Channel and Command Management
Tutorial about implementing channels and command management for ft_irc:
"https://medium.com/@mohamedsarda/ft-irc-channels-and-command-management-ff1ff3758a0b?utm_source=chatgpt.com"

ChatGPT - Readme
