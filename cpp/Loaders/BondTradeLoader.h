#ifndef BONDTRADELOADER_H
#define BONDTRADELOADER_H

#include "../Models/BondTrade.h"
#include "../Models/BondTradeList.h"
#include "ITradeLoader.h"
#include <memory>
#include <string>
#include <vector>

class BondTradeLoader : public ITradeLoader {
private:
  static constexpr char separator = ',';
  std::string dataFile_;

  std::unique_ptr<BondTrade> createTradeFromLine(std::string line);
  void streamTrades(ITradeReceiver &receiver) override;
  void loadTradesFromFile(std::string filename, ITradeReceiver &tradeList);

public:
  std::vector<std::unique_ptr<ITrade>> loadTrades() override;
  std::string getDataFile() const override;
  void setDataFile(const std::string &file) override;
};

#endif // BONDTRADELOADER_H
