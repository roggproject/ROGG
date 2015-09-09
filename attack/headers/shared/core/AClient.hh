#pragma once

/*
  It's abstract class for all clients in the project of the topic "Covert Channel over TCP/IP".
  All clients within this project, especially of the "attack" side must inherit of this abstract class.
*/

class	ISocket;

class	AClient
{
protected:
  ISocket	*_socket;
  
public:
  virtual ~AClient() {}
  //Method for the initialization of the client 
  virtual bool	init(int ac, char *av[]) = 0;
  //Execute the client
  virtual bool	doTheJob() = 0;
};
