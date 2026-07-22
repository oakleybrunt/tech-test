#include "SerialPricer.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include <stdexcept>

std::map<std::string, std::function<std::unique_ptr<IPricingEngine>()>> engineRegistry {
    { "FxPricingEngine", [] { return std::make_unique<FxPricingEngine>(); } },
    { "CorpBondPricingEngine", [] { return std::make_unique<CorpBondPricingEngine>(); } },
    { "GovBondPricingEngine", [] { return std::make_unique<GovBondPricingEngine>(); } },
};

SerialPricer::~SerialPricer() {

}

void SerialPricer::loadPricers() {
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();
    
    for (const auto& configItem : pricerConfig) {
        std::string engineName;
        size_t pos = configItem.getTypeName().rfind('.');
        
        // Try to split pricingEngine by last '.'
        if (pos == std::string::npos) {
            // If no '.' just return typeName
            engineName = configItem.getTypeName();
        } else {
            // return substring after last '.'
            engineName = configItem.getTypeName().substr(pos + 1);
        }
        
        // Look up pricingEngine in registry and return instance
        auto instance = engineRegistry.find(engineName);
        if (instance == engineRegistry.end()) {
            throw std::runtime_error("Unknown pricing engine: " + configItem.getTypeName());
        }

        // Release unique_ptr and return a raw pointer
        IPricingEngine* rawPtr = instance->second().release();
        pricers_.insert({configItem.getTradeType(), rawPtr});
    }
}

void SerialPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                         IScalarResultReceiver* resultReceiver) {
    loadPricers();
    
    for (const auto& tradeContainer : tradeContainers) {
        for (ITrade* trade : tradeContainer) {
            std::string tradeType = trade->getTradeType();
            if (pricers_.find(tradeType) == pricers_.end()) {
                resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
                continue;
            }
            
            IPricingEngine* pricer = pricers_[tradeType];
            pricer->price(trade, resultReceiver);
        }
    }
}
