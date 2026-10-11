#include "ParallelPricer.h"
#include "../Models/ScalarResults.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include <memory>
#include <stdexcept>

ParallelPricer::~ParallelPricer() {}

void ParallelPricer::loadPricers() {
  // If pricers_ is already populated, return
  if (!pricers_.empty()) {
    return;
  }

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

void ParallelPricer::price(
    const std::vector<std::vector<std::unique_ptr<ITrade>>> &tradeContainers,
    IScalarResultReceiver *resultReceiver) {
  // Have to load the pricers first
  loadPricers();

  std::vector<std::future<void>> futures;

  for (const auto &container : tradeContainers) {
    for (const auto &tradePtr : container) {
      const ITrade *trade = tradePtr.get();

      futures.push_back(
          std::async(std::launch::async, [this, trade, resultReceiver]() {
            // 1. Price into a result receiver that only this tasdk can see
            // so no locking is needed during the slow bit
            ScalarResults localResults;

            // find() used instead of [] because [] can insert into the map
            // and want to avoid writing
            auto pricer = pricers_.find(trade->getTradeType());
            if (pricer == pricers_.end()) {
              localResults.addError(
                  trade->getTradeId(),
                  "No Pricing Engines available for this trade type.");
            } else {
              pricer->second->price(*trade, &localResults);
            }

            // 2. Lock the mutex for the copy of priced trade from the
            // localResults. Lock released when this scope ends
            std::lock_guard<std::mutex> lock(resultMutex_);
            for (const auto &result : localResults) {
              if (auto value = result.getResult()) {
                resultReceiver->addResult(result.getTradeId(), *value);
              }
              if (auto error = result.getError()) {
                resultReceiver->addError(result.getTradeId(), *error);
              }
            }
          }));
    }
  }
  for (auto &f : futures) {
    f.get();
  }
}
