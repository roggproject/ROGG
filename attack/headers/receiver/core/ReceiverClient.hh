#pragma once

/*
  It's client class for sending packets with hidden data through the channel TCP/IP.
*/

#include "PortManager.hh"
#include "OutputManager.hh"
#include "AlgorithmManager.hh"
#include "AClient.hh"
#include <map>
#include <string>

class   IAlgorithm;
class   IOutput;
class	ACommand;

class	ReceiverClient : public AClient
{
private:
  //All commands available within the client
  std::map<std::string, ACommand *>	_commands;
  PortManager				_portManager;
  AlgorithmManager			_algorithmManager;
  OutputManager                         _outputManager;
  IAlgorithm				*_algorithm;
  IOutput				*_output;
  
  //Initialize the commands available within the client
  void		initCommands();
  //Check alls commands available within the client
  bool		checkCommands(int ac, char *av[]);
  //Display the usage of the client
  void		help(const char *nameBinary) const;
  
public:
  ReceiverClient();
  virtual ~ReceiverClient();
  //Method for the initialization of the client
  virtual bool  init(int ac, char *av[]);
  //Execute the client
  virtual bool	doTheJob();
};
