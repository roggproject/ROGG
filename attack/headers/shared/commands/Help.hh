#pragma once

/*
  This command allow to display the usage of the client.
*/

#include "ACommand.hpp"

class	Help : public ACommand
{
public:
  Help();
  virtual ~Help();
  //Check the syntax of the command Help
  virtual bool          checkSyntax(std::vector<std::string> &args);
};
