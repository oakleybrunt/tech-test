#ifndef PRICINGTRADERECEIVER_H
#define PRICINGTRADERECEIVER_H

#include "IPricingEngine.h"
#include "IScalarResultReceiver.h"
#include "ITrade.h"
#include "ITradeReceiver.h"
#include <map>
#include <memory>
#include <string>

class PricingTradeReceiver : public ITradeReceiver {
public:
  PricingTradeReceiver(
      std::map<std::string, std::unique_ptr<IPricingEngine>> &pricers,
      IScalarResultReceiver *results)
      : pricers_(pricers), results_(results) {}

  // Called by loader once per trade
  void add(std::unique_ptr<ITrade> trade) override {
    auto it = pricers_.find(trade->getTradeType());
    if (it == pricers_.end()) {
      results_->addError(trade->getTradeId(),
                         "No Pricing Engines available for this trade type.");
      return;
    }
    // Since we are giving a unique pointer here, it is destroyed after this
    // point
    it->second->price(*trade, results_);
  }

private:
  std::map<std::string, std::unique_ptr<IPricingEngine>> &pricers_;
  IScalarResultReceiver *results_;
};

#endif
