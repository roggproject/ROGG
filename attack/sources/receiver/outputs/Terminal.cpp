#include "Terminal.hh"
#include "Defines.hh"
#include "Exception.hh"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <unistd.h>

//Initialization of the ressources and properties of the Terminal output
Terminal::Terminal()
  : _name("Terminal"), _minArg(1)
{
  this->_argFormat = "<instant|block>";
  this->_description = "The data is display in terminal, instantly or by single block of 1024 bytes.";
  this->_singleBlock = false;
  this->_blockSize = 0;
  ::memset(this->_block, 0, 1024);
}

Terminal::~Terminal()
{

}

//Check the syntax of the Terminal output
bool                  Terminal::checkSyntax(std::vector<std::string> &args)
{
  std::string argTerminal;
  
  args.erase(args.begin());
  if (args.size() < this->_minArg)
    return (false);
  argTerminal = args.at(0);
  args.erase(args.begin());
  
  //Get and save the type provided
  if (argTerminal == "instant")
    this->_singleBlock = false;
  else if (argTerminal == "block")
    this->_singleBlock = true;
  else
    return (false);
  return (true);
}

//Accessor method for the name of the PlainText input 
const std::string	&Terminal::getName() const
{
  return (this->_name);
}

//Accessor method for arguments format of the PlainText input
const std::string       &Terminal::getArgFormat() const
{
  return (this->_argFormat);
}

//Accessor method for descritpion of the PlainText input 
const std::string       &Terminal::getDescription() const
{
  return (this->_description);
}

//Handle the data for the output
void			Terminal::outputData(const void *data)
{
  if (this->_singleBlock)
    {
      this->_block[this->_blockSize] = *((uint8_t *) data);
      ++this->_blockSize;
      if (this->_blockSize >= 1024)
	{
	  write(1, this->_block, 1024);
	  ::memset(this->_block, 0, 1024);
	  this->_blockSize = 0;
	}
    }
  else
    write(1, data, 1);
}

//Data single block or not
bool			Terminal::isSingleBlock() const
{
  return (this->_singleBlock);
}

//Output datablock timeout
void			Terminal::outputMemoryTimeout()
{
  if ((this->_singleBlock) && (this->_blockSize > 0))
    {
      write(1, this->_block, this->_blockSize);
      ::memset(this->_block, 0, 1024);
      this->_blockSize = 0;
    }
}
