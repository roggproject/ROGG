#pragma once

/*
  It's an interface for all algorithms available in the client.
  All algorithms must inherit of this interface.
*/

#include <string>
#include <vector>

class IInput;
class IOutput;
class ISocket;

class IAlgorithm
{
public:
  virtual ~IAlgorithm() {}
  //Execute the algorithm for hiding data
  virtual bool			executeHiding() = 0;
  //Execute the algorithm for recovering data
  virtual bool			executeRecovering() = 0;
  //Mutator method for the ressources of the algorithm
  virtual void			setRessources(ISocket *, IInput *) = 0;
  //Mutator method for the ressources of the algorithm
  virtual void			setRessources(ISocket *, IOutput *) = 0;
  //Check the syntax of the algorithm
  virtual bool			checkSyntax(std::vector<std::string> &) = 0;
  //Accessor method for the name of the algorithm
  virtual const std::string	&getName() const = 0;
  //Accessor method for arguments format of the algorithm
  virtual const std::string	&getArgFormat() const = 0;
  //Accessor method for descritpion of the algorithm
  virtual const std::string	&getDescription() const = 0;
};
