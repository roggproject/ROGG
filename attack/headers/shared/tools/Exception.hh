#pragma once

/*
  It's a class which encapsulate the std::exception for customize all exceptions.
*/

#include <stdexcept>

class	Exception : public std::exception
{
private:
  const char	*_msg;
  
public:
  Exception(const char *);
  virtual ~Exception() throw();
  
  virtual const char	*what() const throw();
};
