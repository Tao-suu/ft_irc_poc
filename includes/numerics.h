/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbouchar <lbouchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 15:46:56 by lbouchar          #+#    #+#             */
/*   Updated: 2026/09/01 16:03:17 by lbouchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_H
# define NUMERICS_H

/****RPL****/

//RPL_WELCOME (001) "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
# define RPL_WELCOME(client)                    "001 " + client + " :Welcome to the ft_irc Network"


/****ERR****/

// ERR_UNKNOWNERROR (400) "<client> <command> :<info>"
//   :example.com 400 dan!~d@n PACK :Could not process multiple invalid parameters
# define ERR_UNKNOWNERROR(client, commands)     "400 " + client + " " + command + " :Could not process invalid parameters"


#endif