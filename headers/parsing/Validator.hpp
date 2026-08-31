/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 16:44:52 by picheval          #+#    #+#             */
/*   Updated: 2026/08/31 16:09:01 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

# include <iostream>
# include <cstdlib> // EXIT_FAILURE
# include <bits/stdc++.h> // stringstream
# include "validator.h"
# include "Message.hpp"

class Validator {

private:
	// Attributes
	size_t				print_level;
	static const size_t	PARSE_DEBUG_LENGHT = 20;
	static const size_t	PARSE_DEBUG_ALIGN = 60;
	long long int		char_masks[128]; // TODO tableau
	
	// Methods
	virtual bool		start(void) = 0;
	void				print_stack_state(const int nb_already_printed);
	bool				print_lexer_rule(std::string &rule_name, const bool begin, const bool ret = true);
	void				print_ascii(std::string &str, const size_t index, const size_t limit);
	void				print_space(const size_t nb_spaces);
	size_t				print_tabs(const size_t nb_tabs);
	size_t				compute_string_length(std::string str, int index = 0, int limit = -1);
	bool				print_syntax_error_line_cursor(void);

protected:
	// Attributes
	std::string			content;
	size_t				cursor;
	size_t				max_cursor;
	bool				debug;
	
	// Methods
	void				pre_lexer_rule(std::string rule_name);
	bool				post_lexer_rule(std::string rule_name, const size_t old_cursor, const bool ret);
	bool				find_string(std::string str);
	bool				print_syntax_error(void);
	bool				find_nb_char(int mask, int min, int max);
	bool				global_val_ows(void);
	bool				global_val_ws(void);

public:
	// Attributes

	// Constructors / Destructor
	Validator(void);
	Validator(Validator const &other);
	virtual				~Validator(void) = 0;

	// Operators overload
	Validator			&operator=(Validator const &other);

	// Getters

	// Setters

	// Methods
	bool				validateContent(std::string content);
	Message				parseContent(std::string content);
};

#endif
