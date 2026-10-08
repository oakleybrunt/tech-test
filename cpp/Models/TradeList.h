#ifndef TRADELIST_H
#define TRADELIST_H

#include "ITrade.h"
#include "ITradeReceiver.h"
#include <memory>
#include <vector>

class TradeList : public ITradeReceiver {
public:
  TradeList() = default;

  void add(std::unique_ptr<ITrade> trade) override {
    trades_.push_back(std::move(trade));
  }

  size_t size() const { return trades_.size(); }

  ITrade &operator[](size_t index) const { return *trades_[index]; }

  using container = std::vector<std::unique_ptr<ITrade>>;
  using iterator = container::iterator;
  using const_iterator = container::const_iterator;

  iterator begin() { return trades_.begin(); }
  iterator end() { return trades_.end(); }
  const_iterator begin() const { return trades_.begin(); }
  const_iterator end() const { return trades_.end(); }

private:
  container trades_;
};

#endif // TRADELIST_H
