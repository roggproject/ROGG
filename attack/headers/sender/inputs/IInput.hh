#pragma once

/*
  It's an interface for all inputs available in the client.
  All inputs must inherit of this interface. 
*/

#include <string>
#include <vector>

class	IInput
{
public:
  virtual ~IInput() {}
  //Check the syntax of the input
  virtual bool                  checkSyntax(std::vector<std::string> &) = 0;
  //Accessor method for the name of the input
  virtual const std::string     &getName() const = 0;
  //Accessor method for the data provided by the input
  virtual void			*getData() = 0;
  //Accessor method for arguments format of the input
  virtual const std::string     &getArgFormat() const = 0;
  //Accessor method for descritpion of the input
  virtual const std::string     &getDescription() const = 0;
  //Return the size of the data provided by the input
  virtual unsigned int		getSizeData() const = 0;
};
