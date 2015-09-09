#pragma once

#include "Defines.hh"

/*
  It's a class for defining the format of different outputs and inputs.
*/

#include <iostream>
#include <iomanip>

class   Format
{
public:
  virtual ~Format() {}
  //Display the usage of a command with the format chosen
  static  void  displayUsageWithFormat(const std::string &command, const std::string &args, const std::string &description)
  {
    std::cout << " " << std::setw(TAB_ARG_CMD) << std::left << command << std::setw(TAB_DESC_CMD) << std::left << args << description << std::endl;
  }
};
