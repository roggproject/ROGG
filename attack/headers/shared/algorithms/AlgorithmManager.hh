#pragma once

/*
  This class manages all algorithms available in the client.
  It allows to check the syntax of all algorithms, get the algorithms chosen and displays their usage.
*/

#include "ACommand.hpp"
#include <map>

#define	FORMAT_ALGORITHM	"<algorihm> [<args>]"

class	IAlgorithm;

class	AlgorithmManager : public ACommand
{
private:
  //Map of algorithms available within the client
  std::map<std::string, IAlgorithm *>		_algorithms;
  IAlgorithm					*_algorithmUsed;
  
  //Initialization of all algorithms available within the client
  void			initAlgorithms();
  
public:
  AlgorithmManager();
  virtual ~AlgorithmManager();
  //Check the syntax of the command Algorithm Manager and all algorithms available
  virtual bool          checkSyntax(std::vector<std::string> &args);
  //Accessor method for the algorithm used
  IAlgorithm		*getAlgorithm();
  //Accessor method for the arguments format
  std::string		getFormat() const;
  //Display the usage of the command Algorithm Manager and all algorithms available
  virtual void		displayUsage() const;
};
