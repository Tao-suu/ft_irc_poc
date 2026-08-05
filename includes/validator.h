/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:13:14 by picheval          #+#    #+#             */
/*   Updated: 2026/08/03 18:19:30 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_H
# define VALIDATOR_H

# include <iostream>

# define C_NONE				(0 << 0)
# define C_SPACE			(1 << 0)
# define C_TAG_KEY			(1 << 1)
# define C_TAG_VALUE		(1 << 2)
# define C_CMD				(1 << 3)
# define C_PARAM_VALUE		(1 << 4)

# define CLR_RED			"\x1b[1;38;2;255;0;0;49m"
# define CLR_GREEN			"\x1b[32m"
# define CLR_YELLOW			"\x1b[1;38;2;255;255;0;49m"
# define CLR_MAG			"\x1B[35m"
# define CLR_CYAN			"\x1B[36m"
# define CLR_RESET			"\x1b[0m"

#endif