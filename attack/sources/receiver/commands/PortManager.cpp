#include "PortManager.hh"
#include "Defines.hh"
#include "Format.hpp"
#include <regex>
#include <iostream>
#include <iomanip>

//Initialization of the ressources and properties of Port Manager command
PortManager::PortManager()
  : ACommand()
{
  this->_minArg = 1;
  this->_command = "-port";
  this->_mandatory = true;
  this->_argFormat = FORMAT_PORT;
  this->_description = "Port of reception";
}

PortManager::~PortManager()
{

}

//Check the syntax of the command Port Manager
bool			PortManager::checkSyntax(std::vector<std::string> &args)
{
  std::string   port(args.at(1));

  if ((args.size() - 1) < this->_minArg)
    return (false);
  //Check syntax of the port
  if (!std::regex_match(port, std::regex("[0-9]+")))
    return (false);
  this->_port = std::stoi(port);
  this->_used = true;
  args.erase(args.begin(), args.begin() + 2);
  return (true);
}

//Accessor method for the port attribute
int			PortManager::getPort() const
{
  return (this->_port);
}

//Accessor method for format of the command Port Manager 
std::string		PortManager::getFormat() const
{
  return (std::string(this->_command + " " + this->_argFormat));
}

//Display the usage of the Port Manager
void			PortManager::displayUsage() const
{
  std::cout << "You must to provide the reception port with the command below:" << std::endl;
  Format::displayUsageWithFormat(this->_command, this->_argFormat, this->_description);
}
