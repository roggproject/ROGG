#include <sstream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <algorithm>
#include "ToSDetection.hh"
#include "Log.hpp"

using namespace boost::accumulators;

ToSDetection::ToSDetection()
  : ADetection()
{
  this->setPace(50);
  this->setName(std::string("ToSDetection"));
  this->worksWithPair(false);
}

ToSDetection::~ToSDetection()
{
}

void ToSDetection::testToSValues(boost::ptr_vector<State>& states)
{
  boost::ptr_vector<State>::iterator ite = states.begin();

  for (ite; ite != states.end(); ite++)
    {
      unsigned char tos = ite->getTos();
      if (tos != 0)
	{
	  std::stringstream ss;
	  ss << "- ToS Detection - Abnormal ToS: " << (unsigned short)tos << " - from:" << ite->getFrom().to_string() 
	     << " to:" << ite->getTo().to_string() << " port:" << ite->getPort();
	  std::cout << ss.str() << std::endl;
	  Log::get_mutable_instance().write(ss.str());
	}
    }
}

void ToSDetection::findToSVariations(boost::ptr_vector<State>& states)
{
  std::vector<ToSVariation>::iterator ite = this->_variations.begin();
  bool found = false;

  for (ite; ite != this->_variations.end(); ite++)
    {
      if (ite->from == states.front().getFrom() && ite->to == states.front().getTo())
	{
	  this->registerToSVariations((*ite), states);
	  found = true;
	  break;
	}
    }
  if (!found)
    {
      this->_variations.push_back(ToSVariation(states.front().getFrom(), states.front().getTo()));
      this->registerToSVariations(this->_variations.back(), states);
    }
}

void ToSDetection::registerToSVariations(ToSVariation& var, boost::ptr_vector<State>& states)
{
  boost::ptr_vector<State>::iterator ite = states.begin();
  accumulator_set<double, features<tag::mean, tag::variance>> acc;  

  for (ite; ite != states.end(); ite++)
    {
      acc((double)ite->getTos());
    }

  if (var.variance == 0)
    var.variance = variance(acc);
  else
    var.variance = (var.variance + variance(acc)) / 2;
  
  std::stringstream ss;
  ss << "- ToS Detection - pair: " << var.from.to_string() << "->" << var.to.to_string()
     << ": new variance calculated: " << var.variance;
  Log::get_mutable_instance().write(ss.str());
}

void ToSDetection::analyseToSVariations()
{
}

void ToSDetection::work(boost::ptr_vector<State> states)
{
  this->testToSValues(states);
  this->findToSVariations(states);
}
