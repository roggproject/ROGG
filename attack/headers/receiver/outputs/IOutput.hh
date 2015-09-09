
#pragma once

/*
  It's an interface for all outputs available in the client.
  All outputs must inherit of this interface. 
*/

#include <string>
#include <vector>

class	IOutput
{
public:
  virtual ~IOutput() {}
  //Check the syntax of the output
  virtual bool                  checkSyntax(std::vector<std::string> &) = 0;
  //Accessor method for the name of the output
  virtual const std::string     &getName() const = 0;
  //Accessor method for arguments format of the output
  virtual const std::string     &getArgFormat() const = 0;
  //Accessor method for descritpion of the output
  virtual const std::string     &getDescription() const = 0;
  //Handle the data for the output
  virtual void			outputData(const void *) = 0;
  //Data single block or not
  virtual bool                  isSingleBlock() const = 0;
  //Output datablock timeout
  virtual void                  outputMemoryTimeout() = 0;
};
