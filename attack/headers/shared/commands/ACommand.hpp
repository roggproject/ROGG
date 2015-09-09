#pragma once

/*
  This abstract class allow to execute, check and manage all commands available in the client.
  Thanks to this abstract class, all commands are managed to a generic way.
  All commands must inherit of this abstract class.
*/

#include "Format.hpp"
#include <string>
#include <vector>

class	ACommand
{
protected:
  unsigned int	_minArg;
  unsigned int	_maxArg;
  std::string	_command;
  bool		_mandatory;
  bool		_used;
  std::string	_argFormat;
  std::string	_description;
  
public:
  ACommand()
    : _used(false)
  {
    
  }
  
  virtual ~ACommand() { }
  //Accessor methods of all attributes of the abstract class
  virtual unsigned int		getMinArg() const
  {
    return (this->_minArg);
  }

  virtual unsigned int		getMaxArg() const
  {
    return (this->_maxArg);
  }
  
  virtual const std::string	&getCommandName() const
  {
    return (this->_command);
  }

  virtual bool			isMandatory() const
  {
    return (this->_mandatory);
  }

  virtual bool			isUsed() const
  {
    return (this->_used);
  }

  virtual const std::string	&getArgFormat() const
  {
    return (this->_argFormat);
  }

  virtual const std::string	&getDescription() const
  {
    return (this->_description);
  }

  //Check the syntax of the command
  virtual bool			checkSyntax(std::vector<std::string> &) = 0;
  //Display the usage of the command
  virtual void			displayUsage() const
  {
    Format::displayUsageWithFormat(this->_command, this->_argFormat, this->_description);
  }
};
