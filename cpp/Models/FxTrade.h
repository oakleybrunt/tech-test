#ifndef FXTRADE_H
#define FXTRADE_H

#include "BaseTrade.h"
#include <chrono>
#include <stdexcept>

class FxTrade : public BaseTrade {
public:
    static constexpr const char* FxSpotTradeType = "FxSpot";
    static constexpr const char* FxForwardTradeType = "FxFwd";
    
    FxTrade(const std::string& tradeId = "", const std::string& tradeType = FxSpotTradeType) {
        if (!tradeId.empty()) {
            tradeId_ = tradeId;
            tradeType_ = tradeType;
        }
    }
    
    std::string getTradeType() const override {
        return tradeType_;
    }
    
    std::chrono::system_clock::time_point getValueDate() const { return valueDate_; }
    void setValueDate(const std::chrono::system_clock::time_point& date) { valueDate_ = date; }
    
private:
    std::chrono::system_clock::time_point valueDate_;
};

#endif // FXTRADE_H

