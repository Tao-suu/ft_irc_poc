/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 12:16:36 by picheval          #+#    #+#             */
/*   Updated: 2026/08/03 20:06:31 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.h"

void	printTags(std::string word)
{
	std::stringstream		ss(word);
	std::string				tag;
	std::string::size_type	index;

	std::cout << "Tags:" << std::endl;
	while (getline(ss, tag, ';')) {
		std::cout << "\t";
		index = tag.find("=");
		std::cout << "\"" << tag.substr(0, index) << "\": \"";
		if (index != std::string::npos && index + 1 != std::string::npos)
			std::cout << tag.substr(index + 1);
		std::cout << "\"" << std::endl;
	}
}
void	printCommand(char *line)
{
	// std::cout << std::endl << "Line:" << std::endl << "\t" << line << std::endl;

	char	del[] = " ";
	char	*token = strtok(line, del);
	bool	printedCmd = false;

	for (int i = 0; token != NULL; i++)
	{
		if (i == 0 && token[0] == '@')
			printTags(token + 1);
		else if (!printedCmd)
		{
			std::cout << "Command:" << std::endl << "\t" << token << std::endl;
			printedCmd = true;
			token = strtok(NULL, del);
			if (token)
				std::cout << "Parameters:" << std::endl;
			continue ;
		}
		else if (printedCmd)
		{
			if (token[0] == ':')
			{
				std::cout << "\t" << token + 1;
				token = strtok(NULL, del);
				while (token != NULL)
				{
					std::cout << " " << token;
					token = strtok(NULL, del);
				}
				std::cout << std::endl;
				break ;
			}
			std::cout << "\t" << token << std::endl;
		}
		else
		{
			std::cerr << "Error: Invalid command line !!!";
			break ;
		}
		token = strtok(NULL, del);
	}
}

int		main(void)
{
	CommandValidator	cv;
	char				*line;

	while (42)
	{
		line = readline("> ");
		add_history(line);
		if (!line)
			break ;
		if (cv.validateContent(line))
			printCommand(line);
		free(line);
	}
	return (0);
}