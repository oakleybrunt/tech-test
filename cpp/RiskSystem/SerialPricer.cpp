#include "SerialPricer.h"
#include <stdexcept>

SerialPricer::~SerialPricer() {}

void SerialPricer::loadPricers() {
  PricingConfigLoader pricingConfigLoader;
  pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
  PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();

  for (const auto &configItem : pricerConfig) {
    throw std::runtime_error("Not implemented");
  }
}

void SerialPricer::price(
    const std::vector<std::vector<std::unique_ptr<ITrade>>> &tradeContainers,
    IScalarResultReceiver *resultReceiver) {
  loadPricers();

  for (const auto &tradeContainer : tradeContainers) {
    for (const auto &trade : tradeContainer) {
      std::string tradeType = trade->getTradeType();
      if (pricers_.find(tradeType) == pricers_.end()) {
        resultReceiver->addError(
            trade->getTradeId(),
            "No Pricing Engines available for this trade type");
        continue;
      }

      IPricingEngine *pricer = pricers_[tradeType];
      pricer->price(*trade, resultReceiver);
    }
  }
}
