#include "AddressManager.hh"
#include "Defines.hh"
#include "ISocket.hh"
#include "Format.hpp"
#include <regex>
#include <iostream>
#include <iomanip>

//Initialization of the ressources and properties of Address Manager command
AddressManager::AddressManager()
  : ACommand()
{
  this->_minArg = 1;
  this->_command = "-dest";
  this->_mandatory = true;
  this->_argFormat = FORMAT_ADDRESS;
  this->_description = "Address and port of the destination";
}

AddressManager::~AddressManager()
{

}

//Check the syntax of the command Address Manager
bool			AddressManager::checkSyntax(std::vector<std::string> &args)
{
  std::string   address(args.at(1));
  std::string   port;

  if ((args.size() - 1) < this->_minArg)
    return (false);
  //Check if there are the address and port
  if (std::count(address.begin(), address.end(), ':') > 1)
    return (false);
  port = address.substr(address.find(':'));
  address = address.substr(0, address.find(':'));
  port.erase(port.begin());
  //Check syntax of the port
  if (!std::regex_match(port, std::regex("[0-9]+")))
    return (false);
  //Check if the address is valid
  if (!this->_socket->isValidIPAddress(address.c_str()))
    return (false);
  this->_address = address;
  this->_port = std::stoi(port);
  this->_used = true;
  args.erase(args.begin(), args.begin() + 2);
  return (true);
}

//Accessor method for the address attribute
const std::string	&AddressManager::getAddress() const
{
  return (this->_address);
}

//Accessor method for the port attribute
int			AddressManager::getPort() const
{
  return (this->_port);
}

//Mutator method for the socket attribute
void			AddressManager::setSocket(ISocket *socket)
{
  this->_socket = socket;
}

//Accessor method for format of the command Address Manager 
std::string		AddressManager::getFormat() const
{
  return (std::string(this->_command + " " + this->_argFormat));
}

//Display the usage of the command Address Manager
void			AddressManager::displayUsage() const
{
  std::cout << "You must to provide the destination address and port with the command below:" << std::endl;
  Format::displayUsageWithFormat(this->_command, this->_argFormat, this->_description);
}
