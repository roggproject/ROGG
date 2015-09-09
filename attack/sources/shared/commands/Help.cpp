#include "Help.hh"
#include "Defines.hh"
#include <regex>
#include <iostream>
#include <iomanip>

//Initialization of the ressources and properties of Help command
Help::Help()
  : ACommand()
{
  this->_minArg = 0;
  this->_command = "-help";
  this->_mandatory = false;
  this->_argFormat = "";
  this->_description = "Display help option";
}

Help::~Help()
{

}

//Check the syntax of the command Help
bool            Help::checkSyntax(__attribute__((unused))std::vector<std::string> &args)
{
  this->_used = true;
  return (false);
}
