#include "OutputManager.hh"
#include "Terminal.hh" 
#include "Format.hpp"
#include <regex>
#include <iostream>

//Initialization of the ressources and properties of Output Manager command 
OutputManager::OutputManager()
  : ACommand()
{
  this->_minArg = 1;
  this->_command = "-output";
  this->_mandatory = true;
  this->_outputUsed = NULL;
  this->initOutputs();
}

OutputManager::~OutputManager()
{
  //Free the memory for all outputs available
  for (std::map<std::string, IOutput *>::iterator it = this->_outputs.begin(); it != this->_outputs.end(); ++it)
    {
      if (!it->second)
	delete it->second;
    }
  this->_outputs.clear();
}

//Initialization of all outputs available within the client 
void            OutputManager::initOutputs()
{
  IOutput    *tmp = NULL;

  tmp = new Terminal;
  this->_outputs[tmp->getName()] = tmp;
}

//Check the syntax of the command Output Manager and all outputs available
bool	       OutputManager::checkSyntax(std::vector<std::string> &args)
{
  std::map<std::string, IOutput *>::iterator it = this->_outputs.find(args.at(1));

  args.erase(args.begin());
  if (it != this->_outputs.end())
    {
      if (!it->second->checkSyntax(args))
	return (false);
      this->_outputUsed = it->second;
      this->_used = true;
      return (true);
    }
  return (false);
}

//Accessor method for the output used
IOutput		*OutputManager::getOutput()
{
  return (this->_outputUsed);
}

//Accessor method for the arguments format
std::string	OutputManager::getFormat() const
{
  return (std::string(this->_command + " " + FORMAT_OUTPUT));
}

//Display the usage of the command Output Manager and all outputs available
void		OutputManager::displayUsage() const
{
  std::cout << "You must to provite an output within the outputs list below:" << std::endl;
  for (std::map<std::string, IOutput *>::const_iterator it = this->_outputs.cbegin(); it != this->_outputs.cend(); ++it)
    Format::displayUsageWithFormat(it->second->getName(), it->second->getArgFormat(), it->second->getDescription());
}
