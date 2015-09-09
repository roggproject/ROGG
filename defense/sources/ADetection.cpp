#include "ADetection.hh"

const unsigned int ADetection::getPace() const
{
  return this->_pace;
}

void ADetection::setPace(unsigned int val)
{
  this->_pace = val;
}

const std::string &ADetection::getName() const
{
  return this->_name;
}

void ADetection::setName(std::string &name)
{
  this->_name = name;
}

void ADetection::setName(std::string name)
{
  this->_name = name;
}

const bool ADetection::worksWithPair() const
{
  return this->_worksWithPair;
}

void ADetection::worksWithPair(bool val)
{
  this->_worksWithPair = val;
}
