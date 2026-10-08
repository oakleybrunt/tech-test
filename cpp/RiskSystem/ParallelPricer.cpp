#include "ParallelPricer.h"
#include <stdexcept>

ParallelPricer::~ParallelPricer() {}

void ParallelPricer::loadPricers() {
  PricingConfigLoader pricingConfigLoader;
  pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
  PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();

  for (const auto &configItem : pricerConfig) {
    throw std::runtime_error("Not implemented");
  }
}

void ParallelPricer::price(
    const std::vector<std::vector<std::unique_ptr<ITrade>>> &tradeContainers,
    IScalarResultReceiver *resultReceiver) {
  throw std::runtime_error("Not implemented");
}
