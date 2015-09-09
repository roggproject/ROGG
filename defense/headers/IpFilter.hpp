#pragma once

#include <stdexcept>
#include <boost/asio.hpp>
#include "Defines.hh"

using namespace boost;
using namespace boost::asio;

class IpFilter
{
private:
  
  ip::address _mask;
  ip::address _min;

public:
  IpFilter()
    : _mask(), _min()
  {
    this->_mask = ip::address::from_string("255.255.255.255");
    this->_min = ip::address::from_string("0.0.0.0");
  }

  ~IpFilter()
  {
  }

  class IpFormatException : public std::runtime_error
  {
  public:
    explicit IpFormatException(const std::string &message)
      : std::runtime_error(""), msg(message)
    {
    }
    
    explicit IpFormatException(const char *message)
    : std::runtime_error(""), msg(message)
    {}
    
    virtual ~IpFormatException() throw (){}

    virtual const char *what() const throw ()
    {
      return msg.c_str();
    }

  protected:
    std::string msg;
  };

  void set(const std::string &mask)
  {
    ip::address addr = ip::address::from_string(mask);
    
    if (addr.is_v4() || addr.is_v6())
      {
	this->_min = ip::address::from_string("0.0.0.0");
	this->_mask = addr;
      }
    else
      {
	throw new IpFormatException(IP_FORMAT_ERROR);
      }
  }

  void set(const std::string &min, const std::string &max)
  {

    ip::address addmax = ip::address::from_string(max);
    ip::address addmin = ip::address::from_string(min);
    
    if ((addmax.is_v4() || addmax.is_v6()) && (addmin.is_v4() || addmin.is_v6()))
      {
	if (addmax > addmin)
	  {
	    this->_min = addmin;
	    this->_mask = addmax;
	  }
      }
    else
      {
	throw new IpFormatException(IP_FORMAT_ERROR);
      }
  }

  bool match(const std::string &ip)
  {
    ip::address addr = ip::address::from_string(ip);

    if (!addr.is_v4() && !addr.is_v6())
      throw new IpFormatException(IP_FORMAT_ERROR);

    if (this->_min == ip::address::from_string("0.0.0.0"))
      {
	if (addr < this->_mask)
	  return true;
      }
    else
      {
	if (addr < this->_mask && addr > this->_min)
	  return true;
      }
    return false;
  }
};
