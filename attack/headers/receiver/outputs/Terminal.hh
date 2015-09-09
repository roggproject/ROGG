#pragma once

/*
  It's an interface for all outputs available in the client.
  All outputs must inherit of this interface. 
*/

#include "IOutput.hh"
#include <string>
#include <vector>

class	Terminal : public IOutput
{
private:
  std::string			_name;
  unsigned int			_minArg;
  std::string			_argFormat;
  std::string			_description;
  uint8_t			_block[1024];
  bool				_singleBlock;
  unsigned int			_blockSize;
  
public:
  Terminal();
  virtual ~Terminal();
  //Check the syntax of the output
  virtual bool                  checkSyntax(std::vector<std::string> &);
  //Accessor method for the name of the output
  virtual const std::string     &getName() const;
  //Accessor method for arguments format of the output
  virtual const std::string     &getArgFormat() const;
  //Accessor method for descritpion of the output
  virtual const std::string     &getDescription() const;
  //Handle the data for the output
  virtual void			outputData(const void *data);
  //Data single block or not
  virtual bool			isSingleBlock() const;
  //Output datablock timeout
  virtual void			outputMemoryTimeout();
};
