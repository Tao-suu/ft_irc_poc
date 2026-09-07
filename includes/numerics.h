/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbouchar <lbouchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 15:46:56 by lbouchar          #+#    #+#             */
/*   Updated: 2026/09/07 17:43:35 by lbouchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_H
# define NUMERICS_H

# include <string>

/****RPL****/
// # define RPL_WELCOME(client)                    "001 " + client + " :Welcome to the ft_irc Network"


/****ERR****/
# define ERR_BADCHANNELKEY(client, channel)         std::string("475 ") + client + " " + channel + " :Cannot join channel"
# define ERR_BANNEDFROMCHAN (client, channel)       std::string("474 ") + client + " " + channel + " :Cannot join channel"
#endif