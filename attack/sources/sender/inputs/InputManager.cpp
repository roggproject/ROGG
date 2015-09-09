#include "InputManager.hh"
#include "PlainText.hh"
#include "Format.hpp"
#include <regex>
#include <iostream>

//Initialization of the ressources and properties of Input Manager command 
InputManager::InputManager()
  : ACommand()
{
  this->_minArg = 1;
  this->_command = "-input";
  this->_mandatory = true;
  this->_inputUsed = NULL;
  this->initInputs();
}

InputManager::~InputManager()
{
  //Free the memory for all inputs available
  for (std::map<std::string, IInput *>::iterator it = this->_inputs.begin(); it != this->_inputs.end(); ++it)
    {
      if (!it->second)
	delete it->second;
    }
  this->_inputs.clear();
}

//Initialization of all inputs available within the client 
void            InputManager::initInputs()
{
  IInput    *tmp = NULL;

  tmp = new PlainText;
  this->_inputs[tmp->getName()] = tmp;
}

//Check the syntax of the command Input Manager and all inputs available
bool		InputManager::checkSyntax(std::vector<std::string> &args)
{
  std::map<std::string, IInput *>::iterator it = this->_inputs.find(args.at(1));

  args.erase(args.begin());
  if (it != this->_inputs.end())
    {
      if (!it->second->checkSyntax(args))
	return (false);
      this->_inputUsed = it->second;
      this->_used = true;
      return (true);
    }
  return (false);
}

//Accessor method for the input used
IInput		*InputManager::getInput()
{
  return (this->_inputUsed);
}

//Accessor method for the arguments format
std::string	InputManager::getFormat() const
{
  return (std::string(this->_command + " " + FORMAT_INPUT));
}

//Display the usage of the command Input Manager and all inputs available
void		InputManager::displayUsage() const
{
  std::cout << "You must to provite an input within the inputs list below:" << std::endl;
  for (std::map<std::string, IInput *>::const_iterator it = this->_inputs.cbegin(); it != this->_inputs.cend(); ++it)
    Format::displayUsageWithFormat(it->second->getName(), it->second->getArgFormat(), it->second->getDescription());
}
