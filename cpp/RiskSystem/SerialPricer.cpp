#include "SerialPricer.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include <stdexcept>

SerialPricer::~SerialPricer() {};

void SerialPricer::loadPricers() {
  // instantiates each of the required types of pricing engines based on the
  // configuration
  PricingConfigLoader pricingConfigLoader;
  pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
  PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();

  // Make a map between tradeType and pricing engine
  const std::map<std::string, std::function<std::unique_ptr<IPricingEngine>()>>
      pricingEngineFactory = {
          {"HmxLabs.TechTest.Pricers.GovBondPricingEngine",
           [] { return std::make_unique<GovBondPricingEngine>(); }},
          {"HmxLabs.TechTest.Pricers.CorpBondPricingEngine",
           [] { return std::make_unique<CorpBondPricingEngine>(); }},
          {"HmxLabs.TechTest.Pricers.FxPricingEngine",
           [] { return std::make_unique<FxPricingEngine>(); }},
      };

  for (const auto &configItem : pricerConfig) {
    auto factory = pricingEngineFactory.find(configItem.getTypeName());
    if (factory == pricingEngineFactory.end()) {
      throw std::runtime_error("Unknown pricing engine: " +
                               configItem.getTypeName());
    }
    pricers_[configItem.getTradeType()] = factory->second();
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

      auto &pricer = pricers_[tradeType];
      pricer->price(*trade, resultReceiver);
    }
  }
}
