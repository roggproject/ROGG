#include "Storage-TTL.hh"
#include "Checksum.hh"
#include "TypeOfService.hh"
#include "Timestamp.hh"
#include "IPCoverTiming.hh"
#include "AlgorithmManager.hh"
#include "Format.hpp"
#include <regex>
#include <iostream>

//Initialization of the ressources and properties of Algorithm Manager command
AlgorithmManager::AlgorithmManager()
  : ACommand()
{
  this->_minArg = 1;
  this->_command = "-algorithm";
  this->_mandatory = true;
  this->initAlgorithms();
}

AlgorithmManager::~AlgorithmManager()
{
  //Free the memory for all algorithms available
  for (std::map<std::string, IAlgorithm *>::iterator it = this->_algorithms.begin(); it != this->_algorithms.end(); ++it)
    {
      if (!it->second)
	delete it->second;
    }
  this->_algorithms.clear();
}

//Initialization of all algorithms available within the client
void		AlgorithmManager::initAlgorithms()
{
  IAlgorithm	*tmp = NULL;

  tmp = new StorageTTL;
  this->_algorithms[tmp->getName()] = tmp;
  tmp = new Checksum;
  this->_algorithms[tmp->getName()] = tmp;
  tmp = new TypeOfService;
  this->_algorithms[tmp->getName()] = tmp;
  tmp = new Timestamp;
  this->_algorithms[tmp->getName()] = tmp;
  tmp = new IPCoverTiming;
  this->_algorithms[tmp->getName()] = tmp;
}

//Check the syntax of the command Algorithm Manager and all algorithms available 
bool            AlgorithmManager::checkSyntax(std::vector<std::string> &args)
{
  std::map<std::string, IAlgorithm *>::iterator it = this->_algorithms.find(args.at(1));

  args.erase(args.begin());
  if (it != this->_algorithms.end())
    {
      if (!it->second->checkSyntax(args))
	return (false);
      this->_algorithmUsed = it->second;
      this->_used = true;
      return (true);
    }
  return (false);
}

//Accessor method for the algorithm used
IAlgorithm		*AlgorithmManager::getAlgorithm()
{
  return (this->_algorithmUsed);
}

//Accessor method for the arguments format
std::string		AlgorithmManager::getFormat() const
{
  return (std::string(this->_command + " " + FORMAT_ALGORITHM));
}

//Display the usage of the command Algorithm Manager and all algorithms available
void			AlgorithmManager::displayUsage() const
{
  std::cout << "You must to provite an algorithm within the algorithms list below:" << std::endl;
  for (std::map<std::string, IAlgorithm *>::const_iterator it = this->_algorithms.cbegin(); it != this->_algorithms.cend(); ++it)
    Format::displayUsageWithFormat(it->second->getName(), it->second->getArgFormat(), it->second->getDescription());
}
