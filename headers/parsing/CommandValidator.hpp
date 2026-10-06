
#ifndef COMMAND_VALIDATOR_HPP
# define COMMAND_VALIDATOR_HPP

#include "Validator.hpp"

class CommandValidator: public Validator {

private:
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
	// Constructors / Destructor
	CommandValidator(void);
	CommandValidator(CommandValidator const &other);
	~CommandValidator(void);

	// Operators overload
	CommandValidator	&operator=(CommandValidator const &other);
};

#endif
