#include "Defines.hh"
#include "Exception.hh"
#include "SenderClient.hh"
#include <iostream>

//Main function Sender tool
int	main(int ac, char *av[])
{
  AClient	*client = NULL;
  int		ret = 0;

  //Create client
  if ((client = new SenderClient))
    {
      try
	{
	  //Initialization and execution of the client
	  if ((client->init(ac, av)))
	    ret = client->doTheJob();
	}
      catch (Exception &ex)
	{
	  std::cerr << EXCEPTION_ERROR << ex.what() << std::endl;
	}
      delete client;
    }
  return (ret);
}
