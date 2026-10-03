#include "bots.h"

std::vector<std::string> split(const std::string& s, char del)
{
	std::vector<std::string>	splitted;
	std::istringstream			ss(s);
	std::string					buff;

	while (getline(ss, buff, del))
		if (!buff.empty())
			splitted.push_back(buff);

	return splitted;
}

void	extractFromString(std::string &dest, std::string &src, std::string start, std::string end)
{
	size_t	startPos, endPos;

	if ((startPos = src.find(start)) == std::string::npos)
		throw std::string("Unable to extract start from string");
	startPos += start.size();
	if ((endPos = src.find(end, startPos)) == std::string::npos)
		throw std::string("Unable to extract end from string");
	dest = src.substr(startPos, endPos - startPos);
}

std::string	formatDateTime(time_t timeValue, std::string format)
{
	char	buffer[80];
	tm*		timeInfo = localtime(&timeValue); 
	strftime(buffer, sizeof(buffer), format.c_str(), timeInfo); 
	return (buffer);
}
