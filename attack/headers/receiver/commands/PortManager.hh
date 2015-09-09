
#pragma once

/*
  This class is a command which to allow to check and manage the argument "reception port".
  This command allow to set especially the port of the reception.
*/

#include "ACommand.hpp"

#define	FORMAT_PORT	"<port>"

class	PortManager : public ACommand
{
private:
  int			_port;

public:
  PortManager();
  virtual ~PortManager();
  //Check the syntax of the command Port Manager
  virtual bool          checkSyntax(std::vector<std::string> &args);
  //Accessor method for the port attribute
  int			getPort() const;
  //Accessor method for format of the command Port Manager
  std::string		getFormat() const;
  //Display the usage of the command Port Manager
  virtual void		displayUsage() const;
};
