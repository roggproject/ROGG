#pragma once

#include <iostream>
#include <boost/serialization/singleton.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <ctime>
#include "Defines.hh"

using namespace boost::serialization;
using namespace boost::posix_time;

class Log : public singleton<Log>
{
private:
  std::string _logFile = "";

public:

  const std::string &getLogFile() const
  {
    return this->_logFile;
  }

  void setFile(const std::string &path)
  {
    std::ofstream file(path.c_str());
    
    if (file.is_open())
      {
	this->_logFile = path;
      }
    else
      {
	this->write(FILE_ERROR + path);
      }
  }

  const std::string getFormatDate()
  {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
  }

  void write(const std::string &msg)
  {
    

    if (this->getLogFile() != "")
      {
	std::ofstream file(this->getLogFile().c_str(), std::ofstream::out | std::ofstream::app);

	file << "[" << this->getFormatDate() << "] " << msg << std::endl;

	file.close();
      }
    else
      {
	std::cerr << "[" << this->getFormatDate() << "] " << msg << std::endl;
      }
  }
};
