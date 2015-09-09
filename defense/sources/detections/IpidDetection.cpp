#include <sstream>
#include <algorithm>
#include "IpidDetection.hh"
#include "Log.hpp"

IpidDetection::IpidDetection()
  : ADetection()
{
  this->setPace(20);
  this->setName(std::string("IpidDetection"));
  this->worksWithPair(false);
}

IpidDetection::~IpidDetection()
{
}

void IpidDetection::findIpidVariations(boost::ptr_vector<State>& states)
{
  std::vector<IpidVariation>::iterator ite = this->_variations.begin();
  bool found = false;

  for (ite; ite != this->_variations.end(); ite++)
    {
      if (ite->from == states.front().getFrom() && ite->to == states.front().getTo())
	{
	  this->registerIpidVariations((*ite), states);
	  found = true;
	  break;
	}
    }
  if (!found)
    {
      this->_variations.push_back(IpidVariation(states.front().getFrom(), states.front().getTo()));
      this->registerIpidVariations(this->_variations.back(), states);
    }
}

void IpidDetection::registerIpidVariations(IpidVariation& var, boost::ptr_vector<State>& states)
{
  boost::ptr_vector<State>::iterator ite = states.begin();
  unsigned short expected_value = ite->getId();
  var.abnormalInc = 0;

  for (ite; ite != states.end(); ite++)
    {
      if (expected_value != ite->getId())
	{
	  expected_value = ite->getId();
	  var.abnormalInc++;
	}
      expected_value++;
    }
  
  std::stringstream ss;
  ss << "- Ipid Detection - pair: " << var.from.to_string() << "->" << var.to.to_string()
     << ": anomalies detected: " << var.abnormalInc;
  Log::get_mutable_instance().write(ss.str());
}

void IpidDetection::analyseIpidVariations()
{
}

void IpidDetection::work(boost::ptr_vector<State> states)
{
  this->findIpidVariations(states);
}
