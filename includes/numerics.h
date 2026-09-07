/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbouchar <lbouchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 15:46:56 by lbouchar          #+#    #+#             */
/*   Updated: 2026/09/07 15:05:19 by lbouchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_H
# define NUMERICS_H

/****RPL****/
# define RPL_WELCOME(client)                    "001 " + client + " :Welcome to the ft_irc Network"


/****ERR****/
# define ERR_UNKNOWNERROR(client, commands)     "400 " + client + " " + command + " :Could not process invalid parameters"


#endif