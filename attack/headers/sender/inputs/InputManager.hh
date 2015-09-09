#pragma once

/*
  This class manages all inputs available in the client.
  It allows to check the syntax of all inputs, get the data provided by the input chosen and displays their usage.
*/

#include "ACommand.hpp"
#include <map>
#include <string>

#define	FORMAT_INPUT	"<input> [<args>]"

class	IInput;

class	InputManager : public ACommand
{
private:
  //Map of inputs available within the client
  std::map<std::string, IInput *>	_inputs;
  IInput				*_inputUsed;

  //Initialization of all inputs available within the client
  void                  initInputs();
  
public:
  InputManager();
  virtual ~InputManager();
  //Check the syntax of the command Input Manager and all inputs available 
  virtual bool		checkSyntax(std::vector<std::string> &args);
  //Accessor method for the input used
  IInput		*getInput();
  //Accessor method for the arguments format
  std::string           getFormat() const;
  //Display the usage of the command Input Manager and all inputs available
  virtual void		displayUsage() const;
};
