#ifndef FXTRADELOADER_H
#define FXTRADELOADER_H

#include "ITradeLoader.h"
#include "../Models/FxTrade.h"
#include "../Models/FxTradeList.h"
#include <string>
#include <string_view>
#include <vector>

class FxTradeLoader : public ITradeLoader {
private:
    static constexpr std::string_view separator = {"\xC2\xAC", 2};
    std::string dataFile_;
    
public:
    // NOTE: These methods are only here to allow the solution to compile prior to the test being completed.
    FxTrade* createTradeFromLine(std::string line);
    void loadTradesFromFile(std::string filename, FxTradeList& tradeList);
    std::vector<ITrade*> loadTrades() override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
};

#endif // FXTRADELOADER_H
