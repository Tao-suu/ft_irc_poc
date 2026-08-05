/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 12:18:16 by picheval          #+#    #+#             */
/*   Updated: 2026/08/03 19:04:37 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandValidator.hpp"

/******************/
/* Public methods */
/******************/
// Constructors / Destructor
CommandValidator::CommandValidator(void) : Validator()
{
}
CommandValidator::CommandValidator(CommandValidator const &other) : Validator(other)
{
	*this = other;
}
CommandValidator::~CommandValidator(void)
{
}

// Operators overload
CommandValidator	&CommandValidator::operator=(CommandValidator const &other)
{
	if (this != &other)
	{
	}
	return (*this);
}

// Getters

// Setters

// Methods

/*******************/
/* Private methods */
/*******************/
bool	CommandValidator::cv_val_param_middle(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_param_middle");
	if (!this->find_nb_char(C_PARAM_VALUE, 1, -1))
		return (post_lexer_rule("cv_val_param_middle", old_cursor, false));
	while (42)
	{
		// NO : so param is finished, we can quit
		if (!this->find_string(":"))
			break ;
		// 'aaa:' is valid so no error
		this->find_nb_char(C_PARAM_VALUE, 0, -1);
	}
	return (post_lexer_rule("cv_val_param_middle", old_cursor, true));
}
bool	CommandValidator::cv_val_param_end(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_param_end");
	// '::-)' is valid so no error
	this->find_nb_char(C_PARAM_VALUE, 0, -1);
	while (42)
	{
		// If none of C_PARAM_VALUE, SPACE OR : are found, we can quit
		if (!this->find_string(":")
			&& !this->global_val_ws()
			&& !this->find_nb_char(C_PARAM_VALUE, 1, -1))
			break ;
	}
	return (post_lexer_rule("cv_val_param_end", old_cursor, true));
}
bool	CommandValidator::cv_val_parameters(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_parameters");
	while (42)
	{
		// NO WS so no other param, everything is ok
		if (!this->global_val_ws())
			break ;
		if (this->find_string(":"))
		{
			// If : is found, this must be the last parameter
			// so we quit whatever appens
			return (post_lexer_rule("cv_val_parameters", old_cursor, this->cv_val_param_end()));
		}
		if (!this->cv_val_param_middle())
			return (post_lexer_rule("cv_val_parameters", old_cursor, false));
	}
	return (post_lexer_rule("cv_val_parameters", old_cursor, true));
}
bool	CommandValidator::cv_val_command(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_command");
	if (!this->find_nb_char(C_CMD, 1, -1))
		return (post_lexer_rule("cv_val_command", old_cursor, false));
	return (post_lexer_rule("cv_val_command", old_cursor, true));
}
bool	CommandValidator::cv_val_tag_value(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_tag_value");
	// 'key', 'key=' and 'key=toto' are valid
	// so no need to manage errors in find_nb_char
	if (this->find_string("="))
		this->find_nb_char(C_TAG_VALUE, 0, -1);
	return (post_lexer_rule("cv_val_tag_value", old_cursor, true));
}
bool	CommandValidator::cv_val_tag_key(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_tag_key");
	if (!this->find_nb_char(C_TAG_KEY, 1, -1))
		return (post_lexer_rule("cv_val_tag_key", old_cursor, false));
	return (post_lexer_rule("cv_val_tag_key", old_cursor, true));
}
bool	CommandValidator::cv_val_tag(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_tag");
	if (!this->cv_val_tag_key() || !this->cv_val_tag_value())
		return (post_lexer_rule("cv_val_tag", old_cursor, false));
	return (post_lexer_rule("cv_val_tag", old_cursor, true));
}
bool	CommandValidator::cv_val_tags(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_tags");

	// No @ so not a tag / everything is OK
	if (!this->find_string("@"))
		return (post_lexer_rule("cv_val_tags", old_cursor, true));
	// There's a @ but no valid tag so there is an error
	if (!this->cv_val_tag())
		return (post_lexer_rule("cv_val_tags", old_cursor, false));

	while (42)
	{
		// NO ; so no other tag, everything is ok
		if (!this->find_string(";"))
			break ;
		// There's a ; but no valid tag so there is an error
		if (!this->cv_val_tag())
			return (post_lexer_rule("cv_val_tags", old_cursor, false));
	}
	if (!this->global_val_ws())
		return (post_lexer_rule("cv_val_tags", old_cursor, false));
	return (post_lexer_rule("cv_val_tags", old_cursor, true));
}
bool	CommandValidator::cv_val_line(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("cv_val_line");
	this->global_val_ows();
	if (!this->cv_val_tags() || !this->cv_val_command() || !this->cv_val_parameters())
		return (post_lexer_rule("cv_val_line", old_cursor, false));
	this->global_val_ows();
	return (post_lexer_rule("cv_val_line", old_cursor, true));
}

bool	CommandValidator::start(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("start");
	if (this->cv_val_line())
		return (this->post_lexer_rule("start", old_cursor, true));
	return (this->post_lexer_rule("start", old_cursor, false));
}


