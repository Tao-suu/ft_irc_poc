/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 16:44:19 by picheval          #+#    #+#             */
/*   Updated: 2026/08/03 20:05:52 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validator.hpp"

/******************/
/* Public methods */
/******************/
// Constructors / Destructor
Validator::Validator(void)
{
	// C_SPACE:			SP
	// C_TAG_KEY:		[a-zA-Z0-9] -
	// C_TAG_VALUE:		everything but NUL CR LF SEMICOLON and SP
	// C_CMD:			[A-Z]
	// C_PARAM_VALUE:	everything but NUL CR LF COLON and SP

	this->char_masks[0] = C_NONE;	// NUL \0
	this->char_masks[1] = C_TAG_VALUE | C_PARAM_VALUE;	// SOH
	this->char_masks[2] = C_TAG_VALUE | C_PARAM_VALUE;	// STX
	this->char_masks[3] = C_TAG_VALUE | C_PARAM_VALUE;	// ETX
	this->char_masks[4] = C_TAG_VALUE | C_PARAM_VALUE;	// EOT
	this->char_masks[5] = C_TAG_VALUE | C_PARAM_VALUE;	// ENQ
	this->char_masks[6] = C_TAG_VALUE | C_PARAM_VALUE;	// ACK
	this->char_masks[7] = C_TAG_VALUE | C_PARAM_VALUE;	// BEL \a
	this->char_masks[8] = C_TAG_VALUE | C_PARAM_VALUE;	// BS \b
	this->char_masks[9] = C_TAG_VALUE | C_PARAM_VALUE;	// HT \t
	this->char_masks[10] = C_NONE;	// LF \n
	this->char_masks[11] = C_TAG_VALUE | C_PARAM_VALUE;	// VT \v
	this->char_masks[12] = C_TAG_VALUE | C_PARAM_VALUE;	// FF \f
	this->char_masks[13] = C_NONE;	// CR \r
	this->char_masks[14] = C_TAG_VALUE | C_PARAM_VALUE;	// SO
	this->char_masks[15] = C_TAG_VALUE | C_PARAM_VALUE;	// SI
	this->char_masks[16] = C_TAG_VALUE | C_PARAM_VALUE;	// DLE
	this->char_masks[17] = C_TAG_VALUE | C_PARAM_VALUE;	// DC1
	this->char_masks[18] = C_TAG_VALUE | C_PARAM_VALUE;	// DC2
	this->char_masks[19] = C_TAG_VALUE | C_PARAM_VALUE;	// DC3
	this->char_masks[20] = C_TAG_VALUE | C_PARAM_VALUE;	// DC4
	this->char_masks[21] = C_TAG_VALUE | C_PARAM_VALUE;	// NAK
	this->char_masks[22] = C_TAG_VALUE | C_PARAM_VALUE;	// SYN
	this->char_masks[23] = C_TAG_VALUE | C_PARAM_VALUE;	// ETB
	this->char_masks[24] = C_TAG_VALUE | C_PARAM_VALUE;	// CAN
	this->char_masks[25] = C_TAG_VALUE | C_PARAM_VALUE;	// EM
	this->char_masks[26] = C_TAG_VALUE | C_PARAM_VALUE;	// SUB
	this->char_masks[27] = C_TAG_VALUE | C_PARAM_VALUE;	// ESC
	this->char_masks[28] = C_TAG_VALUE | C_PARAM_VALUE;	// FS
	this->char_masks[29] = C_TAG_VALUE | C_PARAM_VALUE;	// GS
	this->char_masks[30] = C_TAG_VALUE | C_PARAM_VALUE;	// RS
	this->char_masks[31] = C_TAG_VALUE | C_PARAM_VALUE;	// US
	this->char_masks[32] = C_SPACE;	// SPACE
	this->char_masks[33] = C_TAG_VALUE | C_PARAM_VALUE;	// !
	this->char_masks[34] = C_TAG_VALUE | C_PARAM_VALUE;	// "
	this->char_masks[35] = C_TAG_VALUE | C_PARAM_VALUE;	// #
	this->char_masks[36] = C_TAG_VALUE | C_PARAM_VALUE;	// $
	this->char_masks[37] = C_TAG_VALUE | C_PARAM_VALUE;	// %
	this->char_masks[38] = C_TAG_VALUE | C_PARAM_VALUE;	// &
	this->char_masks[39] = C_TAG_VALUE | C_PARAM_VALUE;	// '
	this->char_masks[40] = C_TAG_VALUE | C_PARAM_VALUE;	// (
	this->char_masks[41] = C_TAG_VALUE | C_PARAM_VALUE;	// )
	this->char_masks[42] = C_TAG_VALUE | C_PARAM_VALUE;	// *
	this->char_masks[43] = C_TAG_VALUE | C_PARAM_VALUE;	// +
	this->char_masks[44] = C_TAG_VALUE | C_PARAM_VALUE;	// ,
	this->char_masks[45] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// -
	this->char_masks[46] = C_TAG_VALUE | C_PARAM_VALUE;	// .
	this->char_masks[47] = C_TAG_VALUE | C_PARAM_VALUE;	// /
	this->char_masks[48] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 0
	this->char_masks[49] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 1
	this->char_masks[50] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 2
	this->char_masks[51] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 3
	this->char_masks[52] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 4
	this->char_masks[53] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 5
	this->char_masks[54] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 6
	this->char_masks[55] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 7
	this->char_masks[56] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 8
	this->char_masks[57] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// 9
	this->char_masks[58] = C_TAG_VALUE;	// :
	this->char_masks[59] = C_PARAM_VALUE;	// ;
	this->char_masks[60] = C_TAG_VALUE | C_PARAM_VALUE;	// <
	this->char_masks[61] = C_TAG_VALUE | C_PARAM_VALUE;	// =
	this->char_masks[62] = C_TAG_VALUE | C_PARAM_VALUE;	// >
	this->char_masks[63] = C_TAG_VALUE | C_PARAM_VALUE;	// ?
	this->char_masks[64] = C_TAG_VALUE | C_PARAM_VALUE;	// @
	this->char_masks[65] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// A
	this->char_masks[66] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// B
	this->char_masks[67] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// C
	this->char_masks[68] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// D
	this->char_masks[69] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// E
	this->char_masks[70] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// F
	this->char_masks[71] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// G
	this->char_masks[72] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// H
	this->char_masks[73] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// I
	this->char_masks[74] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// J
	this->char_masks[75] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// K
	this->char_masks[76] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// L
	this->char_masks[77] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// M
	this->char_masks[78] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// N
	this->char_masks[79] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// O
	this->char_masks[80] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// P
	this->char_masks[81] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// Q
	this->char_masks[82] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// R
	this->char_masks[83] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// S
	this->char_masks[84] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// T
	this->char_masks[85] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// U
	this->char_masks[86] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// V
	this->char_masks[87] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// W
	this->char_masks[88] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// X
	this->char_masks[89] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// Y
	this->char_masks[90] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE | C_CMD;	// Z
	this->char_masks[91] = C_TAG_VALUE | C_PARAM_VALUE;	// [
	this->char_masks[92] = C_TAG_VALUE | C_PARAM_VALUE;	// '\'
	this->char_masks[93] = C_TAG_VALUE | C_PARAM_VALUE;	// ]
	this->char_masks[94] = C_TAG_VALUE | C_PARAM_VALUE;	// ^
	this->char_masks[95] = C_TAG_VALUE | C_PARAM_VALUE;	// _
	this->char_masks[96] = C_TAG_VALUE | C_PARAM_VALUE;	// `
	this->char_masks[97] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// a
	this->char_masks[98] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// b
	this->char_masks[99] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// c
	this->char_masks[100] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// d
	this->char_masks[101] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// e
	this->char_masks[102] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// f
	this->char_masks[103] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// g
	this->char_masks[104] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// h
	this->char_masks[105] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// i
	this->char_masks[106] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// j
	this->char_masks[107] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// k
	this->char_masks[108] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// l
	this->char_masks[109] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// m
	this->char_masks[110] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// n
	this->char_masks[111] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// o
	this->char_masks[112] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// p
	this->char_masks[113] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// q
	this->char_masks[114] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// r
	this->char_masks[115] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// s
	this->char_masks[116] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// t
	this->char_masks[117] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// u
	this->char_masks[118] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// v
	this->char_masks[119] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// w
	this->char_masks[120] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// x
	this->char_masks[121] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// y
	this->char_masks[122] = C_TAG_KEY | C_TAG_VALUE | C_PARAM_VALUE;	// z
	this->char_masks[123] = C_TAG_VALUE | C_PARAM_VALUE;	// {
	this->char_masks[124] = C_TAG_VALUE | C_PARAM_VALUE;	// |
	this->char_masks[125] = C_TAG_VALUE | C_PARAM_VALUE;	// }
	this->char_masks[126] = C_TAG_VALUE | C_PARAM_VALUE;	// ~
	this->char_masks[127] = C_TAG_VALUE | C_PARAM_VALUE;	// DEL
}
Validator::Validator(Validator const &other)
{
	*this = other;
}
Validator::~Validator(void)
{
}

// Operators overload
Validator	&Validator::operator=(Validator const &other)
{
	if (this != &other)
	{
		this->content = other.content;
		this->cursor = other.cursor;
		this->max_cursor = other.max_cursor;
		this->print_level = other.print_level;
		this->debug = other.debug;
	}
	return (*this);
}

// Getters

// Setters

// Methods
bool		Validator::validateContent(const char *content)
{
	this->content = content;
	this->cursor = 0;
	this->max_cursor = 0;
	this->print_level = 0;
	this->debug = false;
	if (!this->start() || this->content[this->cursor])
	{
		this->print_syntax_error();
		return (false);
	}
	return (true);
}

/*********************/
/* Protected methods */
/*********************/
bool		Validator::global_val_ows(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("global_val_ows");
	this->find_nb_char(C_SPACE, 0, -1);
	return (this->post_lexer_rule("global_val_ows", old_cursor, true));
}
bool		Validator::global_val_ws(void)
{
	size_t	old_cursor;

	old_cursor = this->cursor;
	this->pre_lexer_rule("global_val_ws");
	if (this->find_nb_char(C_SPACE, 1, -1))
		return (this->post_lexer_rule("global_val_ws", old_cursor, true));
	return (this->post_lexer_rule("global_val_ws", old_cursor, false));
}

void		Validator::pre_lexer_rule(std::string rule_name)
{
	if (this->debug)
		this->print_lexer_rule(rule_name, true);
}
bool		Validator::post_lexer_rule(std::string rule_name, const size_t old_cursor, const bool ret)
{
	if (this->debug)
		this->print_lexer_rule(rule_name, false, ret);
	if (this->max_cursor < this->cursor)
		this->max_cursor = this->cursor;
	if (!ret)
		this->cursor = old_cursor;
	return (ret);
}
bool		Validator::find_string(std::string str)
{
	std::string::iterator	ctntIt; 
	size_t					old_cursor;

	old_cursor = this->cursor;
	ctntIt = this->content.begin() + this->cursor;
	pre_lexer_rule(str);
	for (std::string::iterator strIt = str.begin(); strIt != str.end(); strIt++)
	{
		if (ctntIt == this->content.end() || *ctntIt != *strIt)
			return (post_lexer_rule(str, old_cursor, false));
		ctntIt++;
		this->max_cursor++;
	}
	this->cursor += str.length();
	return (post_lexer_rule(str, old_cursor, true));
}
bool		Validator::print_syntax_error(void)
{
	std::cerr << "Error: Syntax error on line 1:" << this->max_cursor + 1;
	std::cerr << " near `";
	if (!this->content[this->max_cursor])
	{
		std::cerr << "newline`" << std::endl;
		print_ascii(this->content, 0, this->content.length());
		return (false);
	}
	return (this->print_syntax_error_line_cursor());
}
bool			Validator::find_nb_char(int mask, int min, int max)
{
	long long	val_char_mask;
	int			i;

	i = 0;
	for (std::string::iterator it = this->content.begin() + this->cursor; it != this->content.end(); it++)
	{
		val_char_mask = this->char_masks[(int)*it];
		if ((val_char_mask & mask) != mask)
			break ;
		if (max > -1 && i == max)
			break ;
		i++;
	}
	if (i < min)
		return (false);
	this->cursor += i;
	return (true);
}


/*******************/
/* Private methods */
/*******************/
void		Validator::print_ascii(std::string &str, const size_t index, const size_t limit)
{
	size_t	i;

	i = 0;
	for (std::string::iterator it = str.begin() + index; it != str.end() && i < limit; it++)
	{
		if (*it == '\n')
			std::cout << "\\n";
		else if (*it == '\t')
			std::cout << "\\t";
		else
			std::cout << *it;
		i++;
	}
}
void		Validator::print_space(const size_t nb_spaces)
{
	size_t	i;

	i = -1;
	while (++i < nb_spaces)
		std::cout << " ";
}
size_t		Validator::print_tabs(const size_t nb_tabs)
{
	print_space(nb_tabs * 2);
	return (nb_tabs * 2);
}
size_t		Validator::compute_string_length(std::string str, int index, int limit)
{
	std::string::iterator	it;
	std::string::iterator	endIt;
	size_t					ret;

	it = str.begin() + index;
	if (limit == -1)
		endIt = str.end();
	else
		endIt = str.begin() + std::min(str.length(), static_cast<size_t>(limit));
	ret = 0;
	for (; it != endIt; it++)
	{
		if (*it == '\n' || *it == '\t')
			ret++;
		ret++;
	}
	return (ret);
}
// TODO: tout revoir (probleme d'affichage du bon nombre de caracteres
// quand on a des \n \t dans la chaine affichee)
void		Validator::print_stack_state(const int nb_already_printed)
{
	size_t	nb_bck;
	size_t	nb_frt;
	size_t	nb_spcs;
	size_t	content_length;

	content_length = this->compute_string_length(this->content);
	nb_frt = Validator::PARSE_DEBUG_LENGHT;
	if (content_length < Validator::PARSE_DEBUG_LENGHT)
		nb_frt = content_length;
	nb_bck = Validator::PARSE_DEBUG_LENGHT;
	if (this->cursor < Validator::PARSE_DEBUG_LENGHT)
		nb_bck = this->cursor;
	nb_bck = this->compute_string_length(this->content, this->cursor - nb_bck, this->cursor);
	nb_spcs = Validator::PARSE_DEBUG_ALIGN - nb_already_printed + Validator::PARSE_DEBUG_LENGHT - nb_bck;
	print_space(nb_spcs);
	std::cout << CLR_GREEN << "(" << CLR_RESET;
	print_ascii(this->content, this->cursor - nb_bck, nb_bck);
	std::cout << CLR_RED << "|" << CLR_RESET;
	print_ascii(this->content, this->cursor, nb_frt);
	std::cout << CLR_GREEN << ")" << CLR_RESET << std::endl;
}
bool			Validator::print_lexer_rule(std::string &rule_name, const bool begin, const bool ret)
{
	size_t	nb_print;
	size_t	rule_name_length;

	rule_name_length = this->compute_string_length(rule_name);
	if (begin)
	{
		nb_print = print_tabs(this->print_level);
		std::cout << "[";
		print_ascii(rule_name, 0, rule_name_length);
		std::cout << "]";
		nb_print += rule_name_length + 2;
		this->print_stack_state(nb_print);
		this->print_level++;
		return (true);
	}
	this->print_level--;
	nb_print = print_tabs(this->print_level);
	std::cout << (ret ? CLR_GREEN : CLR_RED);
	std::cout << "[";
	print_ascii(rule_name, 0, rule_name_length);
	std::cout << "]";
	nb_print += rule_name_length + 2;
	std::cout << CLR_RESET;
	print_stack_state(nb_print);
	return (ret);
}
bool	Validator::print_syntax_error_line_cursor(void)
{
	size_t	content_length;

	content_length = this->compute_string_length(this->content);
	print_ascii(this->content, this->max_cursor, 1);
	std::cerr << "`" << std::endl;
	print_ascii(this->content, 0, this->max_cursor);
	std::cerr << CLR_RED;
	print_ascii(this->content, this->max_cursor, 1);
	std::cerr << CLR_RESET;
	print_ascii(this->content, this->max_cursor + 1, content_length);
	std::cerr << std::endl;
	return (false);
}
