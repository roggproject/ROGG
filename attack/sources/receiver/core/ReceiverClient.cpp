#include "Defines.hh"
#include "Exception.hh"
#include "ReceiverClient.hh"
#include "CUSocket.hh"
#include "Help.hh"
#include "IAlgorithm.hh"
#include <regex>
#include <algorithm>
#include <iostream>

//Initialization of the ressources and properties of client
ReceiverClient::ReceiverClient()
{
  try
    {
      //Create the socket
      this->_socket = new CUSocket;
    }
  catch (Exception &ex)
    {
      std::cerr << EXCEPTION_ERROR << ex.what() << std::endl;
      this->_socket = NULL;
    }
  //Initialization of the ressources
  this->initCommands();
  this->_algorithm = NULL;
  this->_output = NULL;
}

ReceiverClient::~ReceiverClient()
{
  //Free the memory of all commands available within the client
  for (std::map<std::string, ACommand *>::iterator it = this->_commands.begin(); it != this->_commands.end(); ++it)
    {
      if (!it->second->isMandatory())
	delete it->second;
    }
  this->_commands.clear();
  if (this->_socket)
    delete this->_socket;
}

//Initialize the commands available within the client
void		ReceiverClient::initCommands()
{
  Help		*helpCommand = new Help();
  
  this->_commands[this->_portManager.getCommandName()] = &this->_portManager;
  this->_commands[this->_algorithmManager.getCommandName()] = &this->_algorithmManager;
  this->_commands[this->_outputManager.getCommandName()] = &this->_outputManager;
  this->_commands[helpCommand->getCommandName()] = helpCommand;
}

//Method for the initialization of the client
bool		ReceiverClient::init(int ac, char *av[])
{
  //Check if the socket is initialized
  if (!this->_socket)
    throw Exception(INIT_ERROR);
  
  //Check the syntax of the arguments specified with the commands available
  if (!this->checkCommands(ac, av))
    {
      this->help(av[0]);
      return (false);
    }
  
  //Initialization of the ressources
  this->_socket->setPort(this->_portManager.getPort());
  if (!(this->_algorithm = this->_algorithmManager.getAlgorithm()))
    throw Exception(INIT_ALGO);
  if (!(this->_output = this->_outputManager.getOutput()))
    throw Exception(INIT_OUTPUT);
  this->_algorithm->setRessources(this->_socket, this->_output);
  return (true);
}

//Execute the client
bool		ReceiverClient::doTheJob()
{
  bool		ret = false;

  //Check the ressources
  if ((!this->_socket) || (!this->_algorithm) || (!this->_output))
    throw Exception(INIT_ERROR);

  std::cout << STOP_RECEIVER << std::endl;
  std::cout << LISTEN_STARTED << std::endl;
  //Execute the algorithm chosen
  ret = this->_algorithm->executeRecovering();
  if (ret)
    std::cout << std::endl << LISTEN_SUCCESS << std::endl;
  else
    std::cout << std::endl << LISTEN_FAIL << std::endl;
  return (ret);
}


//Check alls commands available within the client
bool		ReceiverClient::checkCommands(int ac, char *av[])
{
  short				cptArgs = 1;
  std::vector<std::string>	commands;
  
  --ac;
  while (cptArgs <= ac)
    {
      commands.push_back(std::string(av[cptArgs]));
      ++cptArgs;
    }
  if (commands.empty())
    return (false);
  while (!commands.empty())
    {
      std::map<std::string, ACommand *>::iterator it = this->_commands.find(commands.at(0));
      if (it != this->_commands.end())
	{
	  if (!it->second->checkSyntax(commands))
	    return (false);
	}
      else
	return (false);
    }
  if ((!this->_portManager.isUsed()) ||
      (!this->_algorithmManager.isUsed()) ||
      (!this->_outputManager.isUsed()))
    return (false);
  return (true);
}

//Display the usage of the client 
void		ReceiverClient::help(const char *nameBinary) const
{
  bool		hasOptions = false;
  
  std::cout << "usage: " << nameBinary << " " << this->_portManager.getFormat() << " " << this->_algorithmManager.getFormat() << " " << this->_outputManager.getFormat() << " " << FORMAT_OPTIONS << std::endl << std::endl;

  this->_portManager.displayUsage();
  std::cout << std::endl;
  this->_algorithmManager.displayUsage();
  std::cout << std::endl;
  this->_outputManager.displayUsage(); 
  for (std::map<std::string, ACommand *>::const_iterator it = this->_commands.cbegin(); it != this->_commands.cend(); ++it)
    {
      if (!it->second->isMandatory())
	{
	  if (!hasOptions)
	    {
	      std::cout << std::endl << TITLE_OPTIONS << std::endl;
	    }
	  it->second->displayUsage();
	}
    }
  std::cout << std::endl;
}
