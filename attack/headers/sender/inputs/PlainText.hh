#pragma once

/*
  It's an input which allow to get the plain text passed by parameter of the command.
*/

#include "IInput.hh"

class	PlainText : public IInput
{
  std::string   _name;
  unsigned int	_minArg;
  char		*_data;
  unsigned int	_sizeData;
  std::string   _argFormat;
  std::string   _description;
  
public:
  PlainText();
  virtual ~PlainText();
  //Check the syntax of the PlainText input
  virtual bool                  checkSyntax(std::vector<std::string> &args);
  //Accessor method for the name of the PlainText input
  virtual const std::string     &getName() const;
  //Accessor method for the data provided by the PlainText input
  virtual void			*getData();
  //Accessor method for arguments format of the PlainText input
  virtual const std::string     &getArgFormat() const;
  //Accessor method for descritpion of the PlainText input 
  virtual const std::string     &getDescription() const;
  //Return the size of the data provided by the PlainText input
  virtual unsigned int          getSizeData() const;
};
