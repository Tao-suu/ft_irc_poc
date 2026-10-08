*This project has been created as part of the 42 curriculum by lbouchar, picheval, tbez--du*

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
- Channel invitations
- Kicking users
- Changing channel topics
- Channel operators
- Channel modes

Bonus : File transfert and bot

# INSTRUCTIONS

Start the IRC server with:
The port must be between 1024 and 65535.
The password is chosen by the server operator and is required by clients when connecting.

1) Launch server
`./ircserv <port> <password>`
For example:
`./ircserv 1234 mypassword`

2) Connecting server with ncat
Using Netcat from another terminal with the port used to launch the server:<br>
`ncat -C <ip> <port>`
**-C** is for --crlf
then PASS, NICK, USER (see explanation below)
For example:
`ncat -C localhost 1234`
`PASS mypassword`
`NICK Moulinette`
`USER t t t t` (or any letters)


3) Connecting server with irssi
`irssi`
then in irssi
`/connect <ip> <port> <password>`
For example:
`/connect localhost 1234 mypassword`
or
`irssi -c <ip> -p <port> -w <password>`

4) Irc commands

Lexic: 
- `:` is used to make a sentence one unique token (`TOPIC #channel :Hello World!`, "Hello World!" will be one unique token)
- `#` is a specifier for a channel:
&nbsp;`JOIN #channel` - without the '#' the second token will not be considered as a channel name
&nbsp;`PRIVMSG #channel :text to send` - without the '#' the second token will be considered as an user nickname


- PASS
Provides the password required by the server, it must match the password used when starting the server.
`PASS <password>`

- NICK
Sets the client's nickname, which identifies the client on the IRC server and must be unique.
`NICK <nickname>`

- USER
Registers the username and real name of the client, they are used by the server to identify the client.
`USER <username> <mode> <unused> <realname>`
Example:
`USER alice 0 * Alice Smith`

- JOIN
Joins one or more IRC channels.
`JOIN <channel>{,<channel>} [<key>{,<key>}]`
Example:
`JOIN #general`
Multiple channels can be joined at once:
`JOIN #general,#gaming`
A channel password can be provided when joining a channel protected by +k:
`JOIN #private secret123`
For multiple channels with different keys:
`JOIN #general,#private key1,key2`

- PRIVMSG
Sends a private message to another user or a message to a channel.
`PRIVMSG <target>{,<target>} <text to be sent>`
`PRIVMSG alice :Hello Alice!`
Message to a channel
`PRIVMSG #general :Hello everyone!`
Multiple targets
`PRIVMSG alice,bob Hello!`

- TOPIC
Displays or changes the topic of a channel.
`TOPIC <channel>`
Displays the current topic:
`TOPIC #general`
To change the topic:
`TOPIC <channel> <topic>`
When topic protection (+t) is enabled, only channel operators are allowed to change the topic.

- LIST
Displays available channels.
You can also request information about specific channels:
`LIST <channel>`
Example:
`LIST #general`
Multiple channels can be requested:
`LIST #general,#gaming`
The server can return information such as the channel name, number of users, and topic.

- INVITE
Invites a user to a channel.
`INVITE <nickname> <channel>`
Example:
`INVITE alice #private`
The invited user can then join the channel, even when the channel has invite-only mode (+i) enabled, subject to the server's implementation.Typically, the command is used by a channel operator.

- PART
Leaves one or more channels.
`PART <channel>{,<channel>}`
Example:
`PART #general`
Multiple channels can be left at once:
`PART #general,#gaming`
A reason can optionally be provided:
`PART #general :Goodbye everyone!`

- KICK
Removes one or more users from a channel.
`KICK <channel> <user> *( "," <user> ) [<comment>]`
Example:
`KICK #general alice`
A reason can be provided:
`KICK #general alice Spamming`
Multiple users can be kicked:
`KICK #general alice,bob`
This command requires appropriate channel operator privileges.

- MODE
The MODE command is used to view or modify channel settings.
`MODE <target> [<modestring> [<mode arguments>...]]`
For channels, the supported modes are:
+i	or -i Invite-only : Only invited users can join the channel
+t	or -t Topic protection : Only operators can change the topic
+k	or -k Channel key :	A password is required to join the channel
+o	or -o Operator : Gives or removes operator privileges
+l	or -l User limit : Sets the maximum number of users

- QUIT
Disconnect from the IRC server


5) IRSSI COMMAND

- `/connect <ip> <port> <password>`
    to join a irc server

- `/join <channel>` (JOIN)
    to join a channel (the '#' specifier is not mandatory)

- `/nick <nickname>` (NICK)
    to change your nickname

- `/msg <channel>/<user>,[<channel>/<user>...] <text_to_send>` (PRIVMSG)
    to send a message to one or many channel/user (the '#' specifier is necessary for channel, and the ':' one is not)
    you can use `/msg <text_to_send>` if you are on the channel or private message windows

- `/topic <channel> <topic>` (TOPIC)
    to change topic of a channel
    you can use `/topic <topic>` if you are on the channel windows

- `/kick <channel> <user>` (KICK)
    to kick a user from a channel
    you can use `/kick <user>` if you are on the channel windows

- `/mode <channel> <modekey> [<key>...]` (MODE)
    to change mode of a channel
    you can use `/mode <modekey> [<key>...]` if you are on the channel windows

- `/list [<channel>...] -y` (LIST)
    to list channel and number of user of each channel

- `/invite <channel> <user>` (INVITE)
    to invite a user on a channel
    you can use `/invite <user>` if you are on the channel windows

- `/part <channel>` (PART)
    to quit a channel
    you can use `/part` if you are on the channel windows

- `/quit` (QUIT)
    to quit irssi

6) BONUS

- File transfert
Only with IRSSI and 2 clients open
sender :
`/dcc send <nickname> <path_to_file>`
receiver:
`/dcc get <nickname> <filename>`
For example:
&nbsp;`/dcc send moulinette /home/goinfre/ft_irc/README.md`
&nbsp;`/dcc get norminet README.md`

- BOT
For security reasons, API keys must be put manuelly in bots.hpp.
Bots are clients, you need to launch them with
make ircbots:
&nbsp;`./ircbots localhost 1234 toto`
Then in your first client terminal:
&nbsp;`PRIVMSG <bot> <arg>`
BOT WEATHER
Requests weather information for a city.
`PRIVMSG WeatherBot <city>`
Example:
`PRIVMSG WeatherBot Paris`
BOT CHATGPT
Sends a message to the ChatGPT-powered bot.
`PRIVMSG ChatyBot :<text>`
Example:
`PRIVMSG ChatyBot :Explain how IRC channels work ?`
6) CLOSE THE PROGRAMME
server and nc: ^C
`IRSSI: /QUIT`

# RESOURCES

- [Modern IRC documentation](https://modern.ircdocs.horse/)
- [IRSSI documentation](https://irssi.org/documentation/manual/)
- [Tutorial about implementing channels and command management for ft_irc](https://medium.com/@mohamedsarda/ft-irc-channels-and-command-management-ff1ff3758a0b)
- [API ChatyBot](https://console.groq.com/docs/overview) 
- [API WheatherBot](https://openweathermap.org/api/one-call-4?collection=one_call_api)
- ChatGPT - structure of the Readme