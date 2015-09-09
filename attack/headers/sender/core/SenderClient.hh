#pragma once

/*
  It's client class for sending packets with hidden data through the channel TCP/IP.
*/

#include "AddressManager.hh"
#include "InputManager.hh"
#include "AlgorithmManager.hh"
#include "AClient.hh"
#include <map>
#include <string>

class   IAlgorithm;
class   IInput;
class	ACommand;

class	SenderClient : public AClient
{
private:
  //All commands available within the client
  std::map<std::string, ACommand *>	_commands;
  AddressManager			_addressManager;
  AlgorithmManager			_algorithmManager;
  InputManager				_inputManager;
  IAlgorithm				*_algorithm;
  IInput				*_input;

  //Initialize the commands available within the client
  void		initCommands();
  //Check alls commands available within the client
  bool		checkCommands(int ac, char *av[]);
  //Display the usage of the client
  void		help(const char *nameBinary) const;
  
public:
  SenderClient();
  virtual ~SenderClient();
  //Method for the initialization of the client
  virtual bool  init(int ac, char *av[]);
  //Execute the client
  virtual bool	doTheJob();
};
