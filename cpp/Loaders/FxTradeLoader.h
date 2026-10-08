#ifndef FXTRADELOADER_H
#define FXTRADELOADER_H

#include "../Models/FxTrade.h"
#include "../Models/FxTradeList.h"
#include "ITradeLoader.h"
#include <string>
#include <vector>

class FxTradeLoader : public ITradeLoader {
private:
  static constexpr char separator = u'\u00AC';
  std::string dataFile_;

public:
  std::vector<std::unique_ptr<ITrade>> loadTrades() override;
  std::string getDataFile() const override;
  void setDataFile(const std::string &file) override;
  void loadTradesFromFile(std::string filename, FxTradeList &tradeList);
  std::unique_ptr<FxTrade> createTradeFromLine(std::string line);
};

#endif // FXTRADELOADER_H
