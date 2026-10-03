#include "bots.h"

void	weather_bot(Bot &bot, std::string &senderNick, std::string params)
{
	// std::cout << bot << " => " << senderNick << " => " << params << std::endl;

	std::string	response, queryParam, query;
	std::string	host = "api.openweathermap.org";
	std::string	post = "80";
	std::string	apiKey = "6703273dcb390b8667017fa539de9e05";

	// Retrieve coords from location
	// http://api.openweathermap.org/geo/1.0/direct?q={city name},{state code},{country code}&limit={limit}&appid={API key}
	std::string	city = (params.find(":") == 0 ? params.substr(1) : params);
	queryParam = std::string("q=") + city;
	query = std::string("/geo/1.0/direct?") + queryParam + std::string("&limit=1&appid=") + apiKey;

	try {
		sendHttpRequest(host, post, query, response);
	} catch(std::string &msg) {
		sendPrivmsg(bot, senderNick, "Unable to contact weather service :(");
		return ;
	}
	// std::cout << response << std::endl;
	std::string	lat, lon;
	try {
		extractFromString(lat, response, "\"lat\":", ",");
		extractFromString(lon, response, "\"lon\":", ",");
	} catch(std::string &msg) {
		sendPrivmsg(bot, senderNick, "Invalid city name");
		return ;
	}
	// std::cout << "City: " << city << " / lat: " << lat << " / lon: " << lon << std::endl;

	// Retrieve weather from coords
	// https://api.openweathermap.org/data/4.0/onecall/current?lat={lat}&lon={lon}&appid={API key}
	queryParam = std::string("lat=") + lat + std::string("&lon=") + lon;
	query = std::string("/data/4.0/onecall/current?") + queryParam + std::string("&appid=") + apiKey + std::string("&units=metric");
	try {
		sendHttpRequest(host, post, query, response);
	} catch(std::string &msg) {
		sendPrivmsg(bot, senderNick, "Unable to contact weather service :(");
		return ;
	}
	// std::cout << response << std::endl;
	std::string	timezone, dt, sunrise, sunset, temp, feels_like, pressure, humidity, clouds, wind_speed, wind_deg, mainW, description;
	try {
		extractFromString(timezone, response, "\"timezone\":\"", "\",");
		extractFromString(dt, response, "\"dt\":", ",");
		extractFromString(sunrise, response, "\"sunrise\":", ",");
		extractFromString(sunset, response, "\"sunset\":", ",");
		extractFromString(temp, response, "\"temp\":", ",");
		extractFromString(feels_like, response, "\"feels_like\":", ",");
		extractFromString(pressure, response, "\"pressure\":", ",");
		extractFromString(humidity, response, "\"humidity\":", ",");
		extractFromString(clouds, response, "\"clouds\":", ",");
		extractFromString(wind_speed, response, "\"wind_speed\":", ",");
		extractFromString(wind_deg, response, "\"wind_deg\":", ",");
		extractFromString(mainW, response, "\"main\":\"", "\",");
		extractFromString(description, response, "\"description\":\"", "\",");
	} catch(std::string &msg) {
		sendPrivmsg(bot, senderNick, "Invalid weather");
		return ;
	}
	sendPrivmsg(bot, senderNick, std::string("City: ") + city + std::string(" / Timezone: ") + timezone + std::string(" / Date: ") + formatDateTime(std::atoll(dt.c_str()), DATE_FORMAT));
	sendPrivmsg(bot, senderNick, std::string("Current hour: ") + formatDateTime(std::atoll(dt.c_str()), HOUR_FORMAT) + std::string(" / Sunrise: ") + formatDateTime(std::atoll(sunrise.c_str()), HOUR_FORMAT) + std::string(" / Sunset: ") + formatDateTime(std::atoll(sunset.c_str()), HOUR_FORMAT));
	sendPrivmsg(bot, senderNick, mainW + std::string(": ") + description);
	sendPrivmsg(bot, senderNick, std::string("Temperature: ") + temp + std::string("C / FeelsLike: ") + feels_like + std::string("C"));
	sendPrivmsg(bot, senderNick, std::string("Pressure: ") + pressure + std::string("hPa / Humidity: ") + humidity + std::string("% / Clouds: ") + clouds + std::string("%"));
	sendPrivmsg(bot, senderNick, std::string("WindSpeed: ") + wind_speed + std::string("m/s / WindDirection: ") + wind_deg);

	// {"lat":48.8589,"lon":2.32,"timezone":"Europe/Paris","timezone_offset":7200,"data":[{"dt":1791035918,"sunrise":1791006760,"sunset":1791048406,"temp":22.63,"feels_like":22.23,"pressure":1028,"humidity":49,"dew_point":11.38,"uvi":1.78,"clouds":93,"visibility":10000,"wind_speed":4.12,"wind_deg":10,"weather":[{"id":804,"main":"Clouds","description":"couvert","icon":"04d"}]}]}

}
void	chaty_bot(Bot &bot, std::string senderNick, std::string params)
{
	std::cout << bot << " => " << senderNick << " => " << params << std::endl;
	sendPrivmsg(bot, senderNick, "ChatyBot is sleeping, leave him alone please");
}
void	receiveDatas(Bot &bot)
{
	char	buffer[2048];
	std::memset(buffer, 0, 2048);

	ssize_t bytes = recv(bot.socketFd, buffer, 2048, 0);
	if (bytes <= 0) throw std::string("Connection lost");
	if (bytes > 0) bot.receiveBuffer.append(buffer, bytes); 

	size_t  pos;
	while ((pos = bot.receiveBuffer.find(SEPARATOR)) != std::string::npos)
	{
		std::string line = bot.receiveBuffer.substr(0, pos);
		bot.receiveBuffer = bot.receiveBuffer.substr(pos + SEPARATOR.size(), bot.receiveBuffer.size() - (pos + SEPARATOR.size()));

		// std::cout << line << std::endl;
		if ((pos = line.find(WELCOME)) == 0) {
			std::vector<std::string>	splitted = split(line, ' ');
			std::cout << bot.name << " successfuly connected to " << splitted[splitted.size() - 2] << " !" << std::endl;
			continue ;
		}
		if ((pos = line.find(PRIMSVMG)) == std::string::npos) continue ;

		std::string cmd = line.substr(pos + PRIMSVMG.size(), line.size() - (pos + PRIMSVMG.size()));
		std::string	senderNick = line.substr(1, line.find("!") - 1);
		if ((pos = cmd.find(WEATHER_BOT_SEP)) == 0)
			weather_bot(bot, senderNick, cmd.substr(pos + WEATHER_BOT_SEP.size() , cmd.size() - (pos + WEATHER_BOT_SEP.size())));
		else if ((pos = cmd.find(CHATY_BOT_SEP)) == 0)
			chaty_bot(bot, senderNick, cmd.substr(pos + CHATY_BOT_SEP.size(), cmd.size() - (pos + CHATY_BOT_SEP.size())));
		else
			std::cerr << "BAD BOT CMD" << std::endl; // Impossible (sauf si le dev est en carton...)
	}
}

void	sendDatas(Bot &bot)
{
	if (bot.sendList.size() == 0) return ;
	std::string	toSend = bot.sendList.front() + SEPARATOR;
	if (send(bot.socketFd, toSend.c_str(), toSend.size(), 0) < 0)
		throw std::string("Failed to send message from ") + bot.name + std::string("(") + bot.sendList.front() + std::string(")");
	bot.sendList.pop_front();
}

void	startBots(std::vector<Bot> &bots)
{
	std::vector<pollfd>	pollFds;

	for (std::vector<Bot>::iterator it = bots.begin(); it != bots.end(); it++) {
		pollfd	pollFd;
		pollFd.fd = (*it).socketFd;
		pollFd.events = POLLIN | POLLOUT;
		pollFd.revents = 0;
		pollFds.push_back(pollFd);
	}

	while (1)
	{
		if (poll(&pollFds[0], pollFds.size(), -1) == -1)
		{
			if (errno == EINTR) continue;
			throw std::string("Poll error !");
		}

		for (size_t i = 0; i < pollFds.size(); i++)
		{
			if (pollFds[i].revents == 0) continue;					// Nothing append
			if (pollFds[i].revents & POLLIN) receiveDatas(bots[i]);	// Can receive datas
			if (pollFds[i].revents & POLLOUT) sendDatas(bots[i]);	// Can send datas

			// Si poll reçoit une erreur, alors on kill tous les bots (vu qu'ils sont tous liés au même server)
			if (pollFds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) throw std::string("Connection lost");
		}
	}
}

void	connectBots(std::vector<Bot> &bots, int ac, char **av)
{
	if (ac != 4)
	{
		std::cerr << "Missing or too many parameters" << std::endl;
		std::cerr << "Usage: " << av[0] << " IP port password" << std::endl;
		throw std::string("");
	}

	// Creating host, port & password
	std::string host(av[1]);
	std::string port(av[2]);
	std::string password(av[3]);

	// Connecting each bot
	for (std::vector<Bot>::iterator it = bots.begin(); it != bots.end(); it++)
	{
		// Connection to server
		std::cout << "Connecting " << (*it).name << " to " << host << ":" << port << "..." << std::endl;
		connectToServer(host, port, (*it).socketFd);
   		fcntl((*it).socketFd, F_SETFL, O_NONBLOCK);
		std::cout << (*it).name << " successfuly connected to " << host << ":" << port << " !" << std::endl;

		// Creating connection messages
		(*it).sendList.push_back(std::string("PASS ") + password);
		(*it).sendList.push_back(std::string("NICK ") + (*it).name);
		(*it).sendList.push_back(std::string("USER a a a a"));
	}
	std::cout << std::endl;
}

int	main(int ac, char **av)
{
	std::cout << "=========================" << std::endl;
	std::cout << "=== Welcome to bots ! ===" << std::endl;
	std::cout << "=========================" << std::endl << std::endl;

	std::vector<Bot>	bots;
	bots.push_back(Bot(WEATHER_BOT_NAME));
	bots.push_back(Bot(CHATY_BOT_NAME));

	try {
		connectBots(bots, ac, av);
		startBots(bots);
	} catch (std::string &msg) {
		std::cerr << msg << std::endl;
	}

	for (std::vector<Bot>::iterator it = bots.begin(); it != bots.end(); it++)
		if ((*it).socketFd > -1)
			close((*it).socketFd);
	return (0);
}