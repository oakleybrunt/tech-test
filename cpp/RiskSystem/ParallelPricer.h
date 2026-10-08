#ifndef PARALLELPRICER_H
#define PARALLELPRICER_H

#include "../Models/IPricingEngine.h"
#include "../Models/IScalarResultReceiver.h"
#include "../Models/ITrade.h"
#include "PricingConfigLoader.h"
#include <future>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class ParallelPricer {
private:
  std::map<std::string, IPricingEngine *> pricers_;
  std::mutex resultMutex_;

  void loadPricers();

public:
  ~ParallelPricer();

  void price(
      const std::vector<std::vector<std::unique_ptr<ITrade>>> &tradeContainers,
      IScalarResultReceiver *resultReceiver);
};

#endif // PARALLELPRICER_H
