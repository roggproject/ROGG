#include "PlainText.hh"
#include "Defines.hh"
#include "Exception.hh"
#include <iostream>
#include <cstring>
#include <iomanip>

//Initialization of the ressources and properties of the PlainText input
PlainText::PlainText()
  : _name("PlainText"), _minArg(1)
{
  this->_argFormat = "<data>";
  this->_description = "The data input is the argument provided";
}

PlainText::~PlainText()
{
  //Delete the data provided by the input
  delete this->_data;
}

//Check the syntax of the PlainText input
bool                  PlainText::checkSyntax(std::vector<std::string> &args)
{
  std::string argPlainText;
  
  args.erase(args.begin());
  if (args.size() < this->_minArg)
    return (false);
  argPlainText = args.at(0);
  args.erase(args.begin());
  
  //Get and save the data provided by the input
  this->_data = new char[argPlainText.length() + 1];
  if (!::memcpy(this->_data, argPlainText.c_str(), argPlainText.length() + 1))
    return (false);
  this->_sizeData = argPlainText.length();
  return (true);
}

//Accessor method for the name of the PlainText input 
const std::string	&PlainText::getName() const
{
  return (this->_name);
}

//Accessor method for the data provided by the PlainText input
void			*PlainText::getData()
{
  return (this->_data);
}

//Accessor method for arguments format of the PlainText input
const std::string       &PlainText::getArgFormat() const
{
  return (this->_argFormat);
}

//Accessor method for descritpion of the PlainText input 
const std::string       &PlainText::getDescription() const
{
  return (this->_description);
}

//Return the size of the data provided by the PlainText input
unsigned int          PlainText::getSizeData() const
{
  return (this->_sizeData);
}
