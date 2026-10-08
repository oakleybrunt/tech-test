#ifndef ITRADERECEIVER_H
#define ITRADERECEIVER_H

#include "ITrade.h"

class ITradeReceiver {
public:
  virtual ~ITradeReceiver() = default;
  virtual void add(std::unique_ptr<ITrade> trade) = 0;
};

#endif // ITRADERECEIVER_H
