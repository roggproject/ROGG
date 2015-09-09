#include <iostream>
#include "AnalysisService.hh"

int main(int ac, char **av)
{
  AnalysisService* service = new AnalysisService();

  service->init(ac, av);
  service->start();

  delete service;
}
