#pragma once

#include <iostream>
#include <tins/tins.h>
#include "IpFilter.hpp"
#include "AnalysisManager.hh"

using namespace Tins;

class AnalysisService
{
private:
  IpFilter _filter;
  AnalysisManager _analysisManager;
  std::string _net;
  bool _init;

public:
  AnalysisService();
  AnalysisService(const std::string &);
  virtual ~AnalysisService();

  virtual const std::string &getNetInterface() const;
 
  virtual void setNetInterface(const std::string &);
  virtual void setFilter(const std::string &mask);
  virtual void setFilter(const std::string &min, const std::string &max);

  virtual bool packetSniffedCallback(Packet &);
  virtual void init(int, char **);
  virtual void start();
};
