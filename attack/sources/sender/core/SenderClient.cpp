#include "Defines.hh"
#include "Exception.hh"
#include "SenderClient.hh"
#include "CUSocket.hh"
#include "Help.hh"
#include "IAlgorithm.hh"
#include <regex>
#include <algorithm>
#include <iostream>

//Initialization of the ressources and properties of client
SenderClient::SenderClient()
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
  this->_addressManager.setSocket(this->_socket);
  //Initialization of the ressources
  this->initCommands();
  this->_algorithm = NULL;
  this->_input = NULL;
}

SenderClient::~SenderClient()
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
void		SenderClient::initCommands()
{
  Help		*helpCommand = new Help();
  
  this->_commands[this->_addressManager.getCommandName()] = &this->_addressManager;
  this->_commands[this->_algorithmManager.getCommandName()] = &this->_algorithmManager;
  this->_commands[this->_inputManager.getCommandName()] = &this->_inputManager;
  this->_commands[helpCommand->getCommandName()] = helpCommand;
}

//Method for the initialization of the client
bool		SenderClient::init(int ac, char *av[])
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
  this->_socket->setAddress(this->_addressManager.getAddress());
  this->_socket->setPort(this->_addressManager.getPort());
  if (!(this->_algorithm = this->_algorithmManager.getAlgorithm()))
    throw Exception(INIT_ALGO);
  if (!(this->_input = this->_inputManager.getInput()))
    throw Exception(INIT_INPUT);
  this->_algorithm->setRessources(this->_socket, this->_input);
  return (true);
}

//Execute the client
bool		SenderClient::doTheJob()
{
  bool		ret = false;

  //Check the ressources
  if ((!this->_socket) || (!this->_algorithm) || (!this->_input))
    throw Exception(INIT_ERROR);

  //Execute the algorithm chosen
  ret = this->_algorithm->executeHiding();
  if (ret)
    std::cout << std::endl << SEND_SUCCESS << std::endl;
  else
    std::cout << std::endl << SEND_FAIL << std::endl;
  return (ret);
}


//Check alls commands available within the client
bool		SenderClient::checkCommands(int ac, char *av[])
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
  if ((!this->_addressManager.isUsed()) ||
      (!this->_algorithmManager.isUsed()) ||
      (!this->_inputManager.isUsed()))
    return (false);
  return (true);
}

//Display the usage of the client 
void		SenderClient::help(const char *nameBinary) const
{
  bool		hasOptions = false;
  
  std::cout << "usage: " << nameBinary << " " << this->_addressManager.getFormat() << " " << this->_algorithmManager.getFormat() << " " << this->_inputManager.getFormat() << " " << FORMAT_OPTIONS << std::endl << std::endl;

  this->_addressManager.displayUsage();
  std::cout << std::endl;
  this->_algorithmManager.displayUsage();
  std::cout << std::endl;
  this->_inputManager.displayUsage(); 
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
