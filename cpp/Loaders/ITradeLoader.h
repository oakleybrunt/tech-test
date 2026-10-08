#ifndef ITRADELOADER_H
#define ITRADELOADER_H

#include "../Models/ITrade.h"
#include <memory>
#include <string>
#include <vector>

class ITradeLoader {
public:
  virtual ~ITradeLoader() = default;
  virtual std::vector<std::unique_ptr<ITrade>> loadTrades() = 0;
  virtual std::string getDataFile() const = 0;
  virtual void setDataFile(const std::string &file) = 0;
};

#endif // ITRADELOADER_H
