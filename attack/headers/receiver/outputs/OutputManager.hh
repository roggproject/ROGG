#pragma once

/*
  This class manages all outputs available in the client.
  It allows to check the syntax of all outputs, handle the data through the output chosen and displays their usage.
*/

#include "ACommand.hpp"
#include <map>
#include <string>

#define	FORMAT_OUTPUT	"<output> [<args>]"

class	IOutput;

class	OutputManager : public ACommand
{
private:
  //Map of outputs available within the client
  std::map<std::string, IOutput *>	_outputs;
  IOutput				*_outputUsed;

  //Initialization of all outputs available within the client
  void                  initOutputs();
  
public:
  OutputManager();
  virtual ~OutputManager();
  //Check the syntax of the command Output Manager and all outputs available 
  virtual bool		checkSyntax(std::vector<std::string> &args);
  //Accessor method for the output used
  IOutput		*getOutput();
  //Accessor method for the arguments format
  std::string           getFormat() const;
  //Display the usage of the command Input Manager and all outputs available
  virtual void		displayUsage() const;
};
