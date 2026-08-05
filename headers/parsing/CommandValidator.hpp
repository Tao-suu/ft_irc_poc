/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 12:17:46 by picheval          #+#    #+#             */
/*   Updated: 2026/08/03 18:17:24 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_VALIDATOR_HPP
# define COMMAND_VALIDATOR_HPP

#include "Validator.hpp"

class CommandValidator: public Validator {

private:
	// Attributes

	// Methods
	bool				cv_val_param_middle(void);
	bool				cv_val_param_end(void);
	bool				cv_val_parameters(void);
	bool				cv_val_command(void);
	bool				cv_val_tag_value(void);
	bool				cv_val_tag_key(void);
	bool				cv_val_tag(void);
	bool				cv_val_tags(void);
	bool				cv_val_line(void);
	bool				start(void);

public:
	// Attributes

	// Constructors / Destructor
	CommandValidator(void);
	CommandValidator(CommandValidator const &other);
	~CommandValidator(void);

	// Operators overload
	CommandValidator	&operator=(CommandValidator const &other);

	// Getters

	// Setters

	// Methods

};

#endif
