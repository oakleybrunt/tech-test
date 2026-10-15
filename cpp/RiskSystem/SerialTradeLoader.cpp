#include "SerialTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include <memory>

std::vector<std::unique_ptr<ITradeLoader>>
SerialTradeLoader::getTradeLoaders() {
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

std::vector<std::vector<std::unique_ptr<ITrade>>>
SerialTradeLoader::loadTrades() {
    auto loaders = getTradeLoaders();
    std::vector<std::vector<std::unique_ptr<ITrade>>> result;

    for (auto &loader : loaders) {
        result.push_back(loader->loadTrades());
    }

    return result;
}
