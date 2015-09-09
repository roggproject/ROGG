#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include "State.hh"

class ADetection
{
protected:
  unsigned int _pace;
  std::string _name;
  bool _worksWithPair;

public:
  virtual ~ADetection() {}

  virtual const unsigned int getPace() const;
  virtual void setPace(unsigned int);

  virtual const std::string &getName() const;
  virtual void setName(std::string&);
  virtual void setName(std::string);
  
  virtual const bool worksWithPair() const;
  virtual void worksWithPair(bool);
  
  virtual void work(boost::ptr_vector<State>) = 0; 
};
