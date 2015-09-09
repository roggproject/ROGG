#include <sstream>
#include <algorithm>
#include <set>
#include "TimeTCPDetection.hh"
#include "Log.hpp"

TimeTCPDetection::TimeTCPDetection()
  : ADetection()
{
  this->setPace(50);
  this->setName(std::string("TimeTCPDetection"));
  this->worksWithPair(false);
}

TimeTCPDetection::~TimeTCPDetection()
{
}

void TimeTCPDetection::findTimeTCPVariations(boost::ptr_vector<State>& states)
{
  std::vector<TimeTCPVariation>::iterator ite = this->_variations.begin();
  bool found = false;

  for (ite; ite != this->_variations.end(); ite++)
    {
      if (ite->from == states.front().getFrom() && ite->to == states.front().getTo() && ite->port == states.front().getPort())
	{
	  this->registerTimeTCPVariations((*ite), states);
	  found = true;
	  break;
	}
    }
  if (!found)
    {
      this->_variations.push_back(TimeTCPVariation(states.front().getFrom(), states.front().getTo(), states.front().getPort()));
      this->registerTimeTCPVariations(this->_variations.back(), states);
    }
}

void TimeTCPDetection::registerTimeTCPVariations(TimeTCPVariation& var, boost::ptr_vector<State>& states)
{
  boost::ptr_vector<State>::iterator ite = states.begin();
  for (ite; ite != states.end(); ite++)
    {
      var.timestamps.push_back(ite->getTimestamp());
    }
  this->analyseTimeTCPVariations(var);
}

void TimeTCPDetection::analyseTimeTCPVariations(TimeTCPVariation& var)
{
  std::vector<long>::iterator ite = var.timestamps.begin();
  std::set<long> distinct_values;
  unsigned int total = 0;
  unsigned int distincts = 0;
  double diff = 0;

  for (ite; ite != var.timestamps.end(); ite++)
    {
      distinct_values.insert((*ite));
    }

  total = var.timestamps.size();
  distincts = distinct_values.size();

  if (distincts != 0)
    {
      diff = total / distincts;
      if (diff > 1.92 && diff < 1.96)
	{
	  std::stringstream ss;
	  ss << "- TimeTCP Detection - pair: " << var.from.to_string() << "->" << var.to.to_string()
	     << ": wrong values detected " << diff;
	  Log::get_mutable_instance().write(ss.str());
	  std::cout << ss.str() << std::endl;
	}
    }
}

void TimeTCPDetection::work(boost::ptr_vector<State> states)
{
  this->findTimeTCPVariations(states);
}
