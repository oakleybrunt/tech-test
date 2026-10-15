#include "StreamingTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include "../Models/PricingTradeReceiver.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "PricingConfigLoader.h"
#include <stdexcept>

std::vector<std::unique_ptr<ITradeLoader>>
StreamingTradeLoader::getTradeLoaders() {
    std::vector<std::unique_ptr<ITradeLoader>> loaders;

    std::unique_ptr<BondTradeLoader> bondLoader =
        std::make_unique<BondTradeLoader>();
    bondLoader->setDataFile("TradeData/BondTrades.dat");
    loaders.push_back(std::move(bondLoader));

    std::unique_ptr<FxTradeLoader> fxLoader = std::make_unique<FxTradeLoader>();
    fxLoader->setDataFile("TradeData/FxTrades.dat");
    loaders.push_back(std::move(fxLoader));

    return loaders;
}

void StreamingTradeLoader::loadPricers() {
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

StreamingTradeLoader::~StreamingTradeLoader() {}

void StreamingTradeLoader::loadAndPrice(IScalarResultReceiver *resultReceiver) {
    // Load pricers and get loaders
    loadPricers();
    auto loaders = getTradeLoaders();

    PricingTradeReceiver receiver(pricers_, resultReceiver);

    for (auto &loader : loaders) {
        loader->streamTrades(receiver);
    }
}
