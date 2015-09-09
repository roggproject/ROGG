#include <sstream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <algorithm>
#include "TTLDetection.hh"
#include "Log.hpp"

using namespace boost::accumulators;

TTLDetection::TTLDetection()
  : ADetection()
{
  this->setPace(50);
  this->setName(std::string("TTLDetection"));
  this->worksWithPair(false);
}

TTLDetection::~TTLDetection()
{
}

void TTLDetection::testTTLValues(boost::ptr_vector<State>& states)
{
  boost::ptr_vector<State>::iterator ite = states.begin();

  for (ite; ite != states.end(); ite++)
    {
      unsigned int ttl = ite->getTtl();
      if ((ttl > 1 && ttl <= 30) || (ttl > 64 && ttl <= 98) || (ttl > 128 && ttl<= 225))
	{
	  std::stringstream ss;
	  ss << "- TTL Detection - Abnormal TTL: " << ttl << " - from:" << ite->getFrom().to_string() 
	     << " to:" << ite->getTo().to_string() << " port:" << ite->getPort();
	  std::cout << ss.str() << std::endl;
	  Log::get_mutable_instance().write(ss.str());
	}
    }
}

void TTLDetection::findTTLVariations(boost::ptr_vector<State>& states)
{
  std::vector<TTLVariation>::iterator ite = this->_variations.begin();
  bool found = false;

  for (ite; ite != this->_variations.end(); ite++)
    {
      if (ite->from == states.front().getFrom() && ite->to == states.front().getTo())
	{
	  this->registerTTLVariations((*ite), states);
	  found = true;
	  break;
	}
    }
  if (!found)
    {
      this->_variations.push_back(TTLVariation(states.front().getFrom(), states.front().getTo()));
      this->registerTTLVariations(this->_variations.back(), states);
    }
}

void TTLDetection::registerTTLVariations(TTLVariation& var, boost::ptr_vector<State>& states)
{
  boost::ptr_vector<State>::iterator ite = states.begin();
  accumulator_set<double, features<tag::mean, tag::variance>> acc;  

  for (ite; ite != states.end(); ite++)
    {
      var.max = std::max(var.max, ite->getTtl());
      var.min = std::min(var.min, ite->getTtl());
      acc((double)ite->getTtl());
    }

  if (var.mean == 0)
    var.mean = mean(acc);
  else
    var.mean = (var.mean + mean(acc)) / 2;

  if (var.variance == 0)
    var.variance = variance(acc);
  else
    var.variance = (var.variance + variance(acc)) / 2;
  
  std::stringstream ss;
  ss << "- TTL Detection - pair: " << var.from.to_string() << "->" << var.to.to_string()
     << ": new mean calculated: " << var.mean << " - new variance calculated: " << var.variance;
  Log::get_mutable_instance().write(ss.str());

  if (var.variance >= 0.02)
    {
      std::stringstream ss;
      ss << "- TTL Detection - pair: " << var.from.to_string() << "->" << var.to.to_string()
	 << ": abnormal TTL variations detected (" << var.variance << " variance)";
      std::cout << ss.str() << std::endl;
      Log::get_mutable_instance().write(ss.str());
    }
}

void TTLDetection::analyseTTLVariations()
{
}

void TTLDetection::work(boost::ptr_vector<State> states)
{
  this->testTTLValues(states);
  this->findTTLVariations(states);
}
