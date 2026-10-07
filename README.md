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

2) Connecting server with ncat<br>
Using Netcat from another terminal with the port used to launch the server:<br>
`ncat -C <ip> <port>`<br>
**-C** is for --crlf<br>
then PASS, NICK, USER (see explanation below)<br><br>
For example:<br>
`ncat -C localhost 1234`<br>
`PASS mypassword`<br>
`NICK Moulinette`<br>
`USER t t t t` (or any letters)


3) Connecting server with irssi<br>
`irssi`<br>
then in irssi<br>
`/connect <ip> <port> <password>`<br>
For example:<br>
`/connect localhost 1234 mypassword`<br>
or<br>
`irssi -c <ip> -p <port> -w <password>`<br><br>

4) Irc commands

Lexic: 
- `:` is used to make a sentence one unique token (`TOPIC #channel :Hello World!`, "Hello World!" will be one unique token)<br>
- `#` is a specifier for a channel:<br>
&nbsp;`JOIN #channel` - without the '#' the second token will not be considered as a channel name<br>
&nbsp;`PRIVMSG #channel :text to send` - without the '#' the second token will be considered as an user nickname


- PASS
Provides the password required by the server, it must match the password used when starting the server.<br>
`PASS <password>`

- NICK<br>
Sets the client's nickname, which identifies the client on the IRC server and must be unique.<br>
`NICK <nickname>`

- USER<br>
Registers the username and real name of the client, they are used by the server to identify the client.<br>
`USER <username> <mode> <unused> <realname>`<br>
Example:<br>
`USER alice 0 * Alice Smith`<br>

- JOIN<br>
Joins one or more IRC channels.<br>
`JOIN <channel>{,<channel>} [<key>{,<key>}]`<br>
Example:<br>
`JOIN #general`<br>
Multiple channels can be joined at once:<br>
`JOIN #general,#gaming`<br><br>
A channel password can be provided when joining a channel protected by +k:<br>
`JOIN #private secret123`
For multiple channels with different keys:
`JOIN #general,#private key1,key2`

- PRIVMSG<br>
Sends a private message to another user or a message to a channel.<br>
`PRIVMSG <target>{,<target>} <text to be sent>`<br>
`PRIVMSG alice :Hello Alice!`<br>
Message to a channel<br>
`PRIVMSG #general :Hello everyone!`<br>
Multiple targets<br>
`PRIVMSG alice,bob Hello!`<br>

- TOPIC<br>
Displays or changes the topic of a channel.<br>
`TOPIC <channel>`<br>
Displays the current topic:<br>
`TOPIC #general`<br>
To change the topic:<br>
`TOPIC <channel> <topic>`<br>
When topic protection (+t) is enabled, only channel operators are allowed to change the topic.

- LIST<br>
Displays available channels.<br>
You can also request information about specific channels:<br>
`LIST <channel>`<br>
Example:<br>
`LIST #general`<br>
Multiple channels can be requested:<br>
`LIST #general,#gaming`<br>
The server can return information such as the channel name, number of users, and topic.

- INVITE<br>
Invites a user to a channel.<br>
`INVITE <nickname> <channel>`<br>
Example:<br>
`INVITE alice #private`<br>
The invited user can then join the channel, even when the channel has invite-only mode (+i) enabled, subject to the server's implementation.<br>Typically, the command is used by a channel operator.

- PART<br>
Leaves one or more channels.<br>
`PART <channel>{,<channel>}`<br>
Example:<br>
`PART #general`<br>
Multiple channels can be left at once:<br>
`PART #general,#gaming`<br>
A reason can optionally be provided:<br>
`PART #general :Goodbye everyone!`<br>

- KICK<br>
Removes one or more users from a channel.<br>
`KICK <channel> <user> *( "," <user> ) [<comment>]`<br>
Example:<br>
`KICK #general alice`<br>
A reason can be provided:<br>
`KICK #general alice Spamming`<br>
Multiple users can be kicked:<br>
`KICK #general alice,bob`<br>
This command requires appropriate channel operator privileges.<br>

- MODE<br>
The MODE command is used to view or modify channel settings.<br>
`MODE <target> [<modestring> [<mode arguments>...]]`<br>
For channels, the supported modes are:<br><br>
+i	or -i Invite-only : Only invited users can join the channel<br>
+t	or -t Topic protection : Only operators can change the topic<br>
+k	or -k Channel key :	A password is required to join the channel<Br>
+o	or -o Operator : Gives or removes operator privileges<br>
+l	or -l User limit : Sets the maximum number of users<br>

- QUIT<br>
Disconnect from the IRC server


5) IRSSI COMMAND

- `/connect <ip> <port> <password>`<br>
    to join a irc server

- `/join <channel>` (JOIN)<br>
    to join a channel (the '#' specifier is not mandatory)

- `/nick <nickname>` (NICK)<br>
    to change your nickname

- `/msg <channel>/<user>,[<channel>/<user>...] <text_to_send>` (PRIVMSG)<br>
    to send a message to one or many channel/user (the '#' specifier is necessary for channel, and the ':' one is not)<br>
    you can use `/msg <text_to_send>` if you are on the channel or private message windows

- `/topic <channel> <topic>` (TOPIC)<br>
    to change topic of a channel<br>
    you can use `/topic <topic>` if you are on the channel windows

- `/kick <channel> <user>` (KICK)<br>
    to kick a user from a channel<br>
    you can use `/kick <user>` if you are on the channel windows

- `/mode <channel> <modekey> [<key>...]` (MODE)<br>
    to change mode of a channel<br>
    you can use `/mode <modekey> [<key>...]` if you are on the channel windows

- `/list [<channel>...] -y` (LIST)<br>
    to list channel and number of user of each channel<br>

- `/invite <channel> <user>` (INVITE)<br>
    to invite a user on a channel<br>
    you can use `/invite <user>` if you are on the channel windows

- `/part <channel>` (PART)<br>
    to quit a channel<br>
    you can use `/part` if you are on the channel windows

- `/quit` (QUIT)<br>
    to quit irssi

6) BONUS

- File transfert<br>
Only with IRSSI and 2 clients open<br>
sender :
`/dcc send <nickname> <path_to_file>`<br>
receiver:
`/dcc get <nickname> <filename>`<br>
For example:<br>
&nbsp;`/dcc send moulinette /home/goinfre/ft_irc/README.md`<br>
&nbsp;`/dcc get norminet README.md`

- BOT<br>
For security reasons, API keys must be put manuelly in bots.hpp.<br><br>
Bots are clients, you need to launch them with
make ircbots:<br>
&nbsp;`./ircbots localhost 1234 toto`<br>
Then in your first client terminal:<br>
&nbsp;`PRIVMSG <bot> <arg>`<br><br>
BOT WEATHER<br>
Requests weather information for a city.<br>
`PRIVMSG WeatherBot <city>`<br>
Example:<br>
`PRIVMSG WeatherBot Paris`<br><br>
BOT CHATGPT<br>
Sends a message to the ChatGPT-powered bot.<br>
`PRIVMSG ChatyBot :<text>`<br>
Example:<br>
`PRIVMSG ChatyBot :Explain how IRC channels work ?`<br><br>
6) CLOSE THE PROGRAMME<br>
server and nc: ^C<br>
`IRSSI: /QUIT`<br><br><br>

# RESOURCES

- [Modern IRC documentation](https://modern.ircdocs.horse/)<br>
- [IRSSI documentation](https://irssi.org/documentation/manual/)<br>
- [Tutorial about implementing channels and command management for ft_irc](https://medium.com/@mohamedsarda/ft-irc-channels-and-command-management-ff1ff3758a0b)<br>
- [API ChatyBot](https://console.groq.com/docs/overview) 
- [API WheatherBot](https://openweathermap.org/api/one-call-4?collection=one_call_api)<br>
- ChatGPT - structure of the Readme